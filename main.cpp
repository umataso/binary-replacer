#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <codecvt>
using namespace std;

std::string hex2bin(std::string s);
int createbinary(std::string filename, std::string text);
std::string hutaketa(std::string text);
std::string replaceOtherStr(std::string &replacedStr, std::string from, std::string to);
int main()
{
    string fileName = "";  // 入力ファイル名
    string outfile  = "";  // 出力ファイル名

    string before = ""; // 置換前文字列
    string after  = ""; // 置換後文字列
    
    string before_hex; // 置換前文字列の16進数表記格納用y
    string after_hex; // 置換後文字列の16進数表記格納用

    // 情報の出力
    cout << "\n"+fileName + " -> " + outfile << endl;
    cout << "\nbefore: "+before + "\nafter : "+after << endl;

    // 置換前文字列を16進数表記に変換
    stringstream ss;
    for (const auto &item : before) {
        ss << hex << int(item);
    }
    before_hex = ss.str();

    // 置換後文字列を16進数表記に変換
    stringstream ss1;
    for (const auto &item : after) {
        ss1 << hex << int(item);
    }
    after_hex = ss1.str();

    //ファイル名からバイナリファイルで読み込む
    std::ifstream ifs(fileName, std::ios::binary);

    //読込サイズを調べる。
    ifs.seekg(0, std::ios::end);
    long long int size = ifs.tellg();
    ifs.seekg(0);

    //読み込んだデータをchar型に出力する
    char *data = new char[size];
    ifs.read(data, size);

    //サイズを出力する
    std::cout << "size = "<< size <<"\n" ;
    std::string text;
    std::stringstream hex_zero;
    hex_zero << std::hex << "0";
    for (int i = 1; i < size + 1; i++)
    {
        std::stringstream ss;

        //16進数文字列に変換する。char型をASCIIではなく、数値で出力する場合は先頭に+を付ける
        ss << std::hex << +data[i - 1];

        string test = ss.str();
        //符号付きであるか判定する
        if (test.size() > 5)
        {
            //符号付きの場合fが付くため、fを削除する
            string test = ss.str().erase(0, 6);
            text += test + " ";
        }
        else
        {
            //符号付きでない場合そのまま出力する
            string test = ss.str();
            text += ss.str() + " ";

        }
    }

    delete data; // メモリ解放
    std::string new_text = hutaketa(text); // 2文字毎にスペースを挟む
    std::string backup_text = hex2bin(new_text); // バックアップファイルを作成するために残す(トラブルが起きた場合のために)
    createbinary("backup.bin", backup_text); // バックアップファイルを保存する
    std::string new_hex = regex_replace(new_text, regex(before_hex), after_hex); // 文字列を置換する
    std::string bin_text = hex2bin(new_hex); // 2桁毎にスペースで区切った16進数からバイナリとして保存できる形式に変換する
    
    createbinary(outfile, bin_text); // 置換後のデータを保存する
    //createbinary(outfile, new_text);
    return 0;
}

// 二桁毎にスペースを入れて区切る
std::string hutaketa(std::string text){
    std::string nums = "123456789abcdef0";
    std::string before;
    std::string after;
    std::string new_hex = text;
    for (int i; i <16; i++){
        before = " " + nums.substr(i, 1) + " ";
        after = " 0" + nums.substr(i, 1) + " ";
        new_hex = regex_replace(new_hex, regex(before), after);
        new_hex = regex_replace(new_hex, regex(before), after);
    }
    return regex_replace(regex_replace(new_hex, regex(" "), ""), regex("0A"), "");
}

// 文字列を変換する
std::string replaceOtherStr(std::string &replacedStr, std::string from, std::string to) {
    const unsigned int pos = replacedStr.find(from);
    const int len = from.length();
 
    if (pos == std::string::npos || from.empty()) {
        return replacedStr;
    }
    cout << from << endl;
    cout << to << endl;
    return replacedStr.replace(pos, len, to);
}
 
// 2桁毎にスペースで区切った16進数からバイナリとして保存できる形式に変換する
std::string hex2bin(std::string s) {
    std::string rc;
    int nLen = s.length();
    int tmp;
    for (int i(0); i + 1 < nLen; i += 2) {
        if (std::istringstream(s.substr(i, 2)) >> std::hex >> tmp) {
            rc.push_back(tmp);
        }
    }
    return rc;
}

// バイナリファイルを保存する
int createbinary(std::string filename, std::string text){
    std::ofstream fout;
    fout.open(filename, ios::out|ios::binary|ios::trunc);
    if (!fout) {
        return 1;
    }
    fout << text << std::endl;
    fout.close();
    return 0;
}
