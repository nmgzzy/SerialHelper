#include "language.h"
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "inipp.h"

using std::string;
using std::unordered_map;
using std::vector;

static unordered_map<string, string> s_text_map;
static string s_language_setting = "English";

int InitLanguage(const string &language)
{
    inipp::Ini<char> ini;
    std::ifstream is("res/language.ini");
    static vector<string> avaliable_languages;

    ini.parse(is);
    is.close();
    // ini.strip_trailing_comments();
    int index = 0;

    if (avaliable_languages.empty())
    {
        for (auto const &sec : ini.sections)
        {
            avaliable_languages.emplace_back(sec.first);
        }
    }

    auto iter = std::find(avaliable_languages.begin(), avaliable_languages.end(), language);

    if (iter == avaliable_languages.end())
    {
        return -1;
    }

    s_language_setting = language;
    s_text_map.clear();
    auto section = ini.sections[language];
    for (auto const &kv : section)
    {
        auto ret_pair = s_text_map.insert(kv);
        if (ret_pair.second == false)
        {
            SDL_Log("LanguageInit, insert fail");
        }
        SDL_Log("LanguageInit, %s, %s", kv.first.c_str(), kv.second.c_str());
        // s_text_map.insert_or_assign(kv.first, kv.second); // C++17
    }

    return 0;
}

const string & GetText(const string &key)
{
    auto iter = s_text_map.find(key);
    if (iter != s_text_map.end())
    {
        return iter->second;
    }
    SDL_Log("mtd_GetText, cannot find text");
    return key;
}