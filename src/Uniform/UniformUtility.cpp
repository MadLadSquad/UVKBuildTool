#include "UniformUtility.h"
#include <Generator.hpp>
#include <cstdlib>

std::filesystem::path& UBT::getPath() noexcept
{
    static std::filesystem::path path("../../");
    return path;
}

void UBT::setPath(const char* pt) noexcept
{
    std::string str = pt == nullptr ? "" : pt;
    // An empty path means the working directory. Worth spelling out, because str.back() on an empty string
    // is undefined behaviour and the path comes straight from the command line
    if (str.empty())
        str = "./";
    else if (str.back() != '/')
        str += "/";
    // Cleanup if some stupid Windows user decides to use this
    for (auto& a : str)
        if (a == '\\')
            a = '/';
    getPath() = std::move(std::filesystem::path(str));
}

std::string UBT::loadFileToString(const std::string& p) noexcept
{
    // Opened in binary mode on purpose. In text mode Windows collapses every CRLF on the way in, so read()
    // returns fewer bytes than the tellg() size the buffer was sized from and the tail of the string stays
    // filled with padding - which is where a good part of the "Windows adds random data to our files"
    // folklore comes from. Sizing the result to what was actually read keeps that honest on every platform
    std::ifstream in(p, std::ios::binary);
    if (!in.is_open())
        return "";

    in.seekg(0, std::ios::end);
    const std::streampos size = in.tellg();
    if (size < 0)
        return "";

    std::string buffer(static_cast<size_t>(size), '\0');

    in.seekg(0);
    in.read(buffer.data(), size);
    buffer.resize(static_cast<size_t>(in.gcount()));

    in.close();
    return buffer;
}

ryml::NodeRef UBT::setupWorkdir(const char* x, std::string& name) noexcept
{
    setPath(x);
    return getConfig(name);
}

ryml::NodeRef UBT::getConfig(std::string& name) noexcept
{
    // uvproj.yaml is loaded and parsed exactly once per process. The tree owns the arena that every
    // NodeRef handed out over the run points into, so it must outlive all of them - which also means a
    // single invocation can only ever operate on the project that was set up first.
    static ryml::Tree tree = []
    {
        const std::string string = UBT::loadFileToString((UBT::getPath()/"uvproj.yaml").string());
        if (string.empty())
        {
            std::cout << UBT_COL_ERROR << "Could not load uvproj.yaml" << UBT_COL_END << std::endl;
            std::exit(EXIT_FAILURE);
        }

        auto result = ryml::parse_in_arena(ryml::to_csubstr(string));
        if (result.empty())
        {
            std::cout << UBT_COL_ERROR << "Could not parse uvproj.yaml" << UBT_COL_END << std::endl;
            std::exit(EXIT_FAILURE);
        }
        return result;
    }();

    auto root = tree.rootref();
    auto n = root["name"];
    if (ryml::keyValid(n))
        n.load(&name);
    return root;
}

static const char* parseStatusToString(const UTTE::ParseResultStatus status) noexcept
{
    switch (status)
    {
    case UTTE_PARSE_STATUS_SUCCESS:
        return "success";
    case UTTE_PARSE_STATUS_OUT_OF_BOUNDS:
        return "an array or map was accessed out of bounds";
    case UTTE_PARSE_STATUS_EXPECTED_TERMINATION:
        return "an expression was not terminated";
    case UTTE_PARSE_STATUS_INVALID_VALUE:
        return "an invalid value was given to a function";
    case UTTE_PARSE_STATUS_INVALID_TYPE:
        return "a value of the wrong type was given to a function";
    default:
        return "unknown error";
    }
}

bool UBT::tryLoadTemplate(UTTE::Generator& generator, const std::string& templatePath) noexcept
{
    if (generator.loadFromFile(templatePath) != UTTE_INITIALISATION_RESULT_SUCCESS)
    {
        std::cout << UBT_COL_ERROR << "Could not open the following template file: " << templatePath << UBT_COL_END << std::endl;
        return false;
    }
    return true;
}

void UBT::loadTemplate(UTTE::Generator& generator, const std::string& templatePath) noexcept
{
    if (!tryLoadTemplate(generator, templatePath))
        std::exit(EXIT_FAILURE);
}

const utte_string* UBT::tryParseTemplate(UTTE::Generator& generator, const std::string& context) noexcept
{
    const auto result = generator.parse();
    if (result.status != UTTE_PARSE_STATUS_SUCCESS || result.result == nullptr)
    {
        std::cout << UBT_COL_ERROR << "Could not parse the following template file: " << context << ". Reason: "
                  << parseStatusToString(result.status) << UBT_COL_END << std::endl;
        return nullptr;
    }
    return result.result;
}

bool UBT::tryWriteTemplate(UTTE::Generator& generator, const std::filesystem::path& destination, const std::string& templatePath) noexcept
{
    const auto* parsed = tryParseTemplate(generator, templatePath);
    if (parsed == nullptr)
        return false;

    // std::ios::binary is required, not cosmetic. loadFromFile reads templates in binary mode, so a checkout with
    // core.autocrlf=true hands us content that already has CRLF endings. Writing that back in text mode makes Windows
    // expand the LF of every pair again, producing CRCRLF, which MSVC then reports as "Mac file format detected"(C4335)
    // on the generated headers. In binary mode the template's endings are reproduced verbatim, whichever they are
    std::ofstream stream(destination, std::ios::binary);
    if (!stream.is_open())
    {
        std::cout << UBT_COL_ERROR << "Could not open the following file for writing: " << destination.string() << UBT_COL_END << std::endl;
        return false;
    }

    // Windows really likes fucking up everything we do. Basically, if you don't call "->c_str()", a lot of null
    // terminators will be added to the end of the file. After that, Windows will shit itself and would not be able
    // to read the file. Other applications will not be able to open it too.
    stream << parsed->c_str();
    stream.close();

    // Checked because a full disk or a read only tree otherwise produces a truncated generated file that is
    // reported as a success and only fails much later, when the user tries to build it
    if (!stream)
    {
        std::cout << UBT_COL_ERROR << "Could not write the generated file: " << destination.string() << UBT_COL_END << std::endl;
        return false;
    }
    return true;
}

void UBT::writeTemplate(UTTE::Generator& generator, const std::filesystem::path& destination, const std::string& templatePath) noexcept
{
    if (!tryWriteTemplate(generator, destination, templatePath))
        std::exit(EXIT_FAILURE);
}

bool UBT::tryCreateDirectory(const std::filesystem::path& dir) noexcept
{
    if (std::filesystem::exists(dir))
        return true;

    std::error_code code;
    std::filesystem::create_directory(dir, code);
    if (code)
    {
        std::cout << UBT_COL_ERROR << "Could not create the directory '" << dir.string() << "': " << code.message() << UBT_COL_END << std::endl;
        return false;
    }
    return true;
}

void UBT::createDirectory(const std::filesystem::path& dir) noexcept
{
    if (!tryCreateDirectory(dir))
        std::exit(EXIT_FAILURE);
}

void UBT::createDirectorySymlink(const std::filesystem::path& target, const std::filesystem::path& link) noexcept
{
    std::error_code code;
    std::filesystem::create_directory_symlink(target, link, code);
    if (code)
    {
        std::cout << UBT_COL_ERROR << "Could not link '" << link.string() << "' to '" << target.string() << "': " << code.message()
                  << "\nOn Windows this usually means that the account is missing the 'Create symbolic links' privilege." << UBT_COL_END << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void UBT::ensureDirectorySymlink(const std::filesystem::path& target, const std::filesystem::path& link) noexcept
{
    // symlink_status() does not follow the link, unlike exists(). A dangling link - which is what is left
    // behind when the framework is moved or the tree is copied without its target - looks absent to exists()
    // and the creation below would then fail with "file exists", so it gets replaced instead
    const auto status = std::filesystem::symlink_status(link);
    if (std::filesystem::exists(status))
    {
        // Something real is there: either a working link or a directory the user put there on purpose
        if (!std::filesystem::is_symlink(status) || std::filesystem::exists(link))
            return;

        std::error_code code;
        std::filesystem::remove(link, code);
        if (code)
        {
            std::cout << UBT_COL_ERROR << "Could not remove the broken symlink '" << link.string() << "': " << code.message() << UBT_COL_END << std::endl;
            std::exit(EXIT_FAILURE);
        }
        std::cout << UBT_COL_WARNING << "Replaced the broken symlink '" << link.string() << "'" << UBT_COL_END << std::endl;
    }
    createDirectorySymlink(target, link);
}

void UBT::copyFile(const std::filesystem::path& from, const std::filesystem::path& to, const std::filesystem::copy_options options) noexcept
{
    std::error_code code;
    std::filesystem::copy_file(from, to, options, code);
    if (code)
    {
        std::cout << UBT_COL_ERROR << "Could not copy '" << from.string() << "' to '" << to.string() << "': " << code.message() << UBT_COL_END << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

bool ryml::keyValid(NodeRef ref) noexcept
{
    return !ref.invalid() && ref.readable() && !ref.empty();
}

bool ryml::keyValid(const ConstNodeRef ref) noexcept
{
    return !ref.invalid() && ref.readable() && !ref.empty();
}
