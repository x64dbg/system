#include "plugin.h"

static std::wstring Utf8ToUtf16(const char* str)
{
    std::wstring convertedString;
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    if(requiredSize > 0)
    {
        std::vector<wchar_t> buffer(requiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, &buffer[0], requiredSize);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
}

static bool cbCommand(int argc, char* argv[])
{
    if(argc < 2)
    {
        _plugin_logputs("[" PLUGIN_NAME "] Nothing to execute!");
        return false;
    }

    char cmd[MAX_SETTING_SIZE] = "";
    if(strstr(argv[1], "{"))
    {
        if(!DbgFunctions()->StringFormatInline(argv[1], _countof(cmd), cmd))
        {
            _plugin_logputs("[" PLUGIN_NAME "] Failed to format command!");
            return false;
        }
    }
    else
        strncpy_s(cmd, argv[1], _TRUNCATE);

    _plugin_logprintf("[" PLUGIN_NAME "] %s\n", cmd);
    return _wsystem(Utf8ToUtf16(cmd).c_str()) == 0;
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    if(!_plugin_registercommand(pluginHandle, PLUGIN_NAME, cbCommand, false))
        _plugin_logputs("[" PLUGIN_NAME "] Error registering the \"" PLUGIN_NAME "\" command!");
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
    return true;
}

//Do GUI/Menu related things here.
void pluginSetup()
{
}
