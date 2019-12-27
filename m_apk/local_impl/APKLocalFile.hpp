#pragma once

#ifndef CSIGNAPK_APKLOCALFILE_HPP
#define CSIGNAPK_APKLOCALFILE_HPP

#include <sqlite3.h>

#include "bean/APKLocalBeanFileCon.hpp"
#include "bean/APKLocalBeanFileVar.hpp"

namespace m
{

inline namespace apk
{

class APKLocalFile
        : public APKLocalActionI<APKLocalBeanFileVar, APKLocalBeanFileCon>
{
private:
    constexpr static Jchar TABLE_SIGN[] = "/1631962F301C59AC3ECC3DE66A35DD5A";

    constexpr static Jchar T_CREATE[] =
                                   "CREATE TABLE T_APK("
                                   "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "NAME TEXT,"
                                   "SIGN BLOB"
                                   ");";

    constexpr static Jchar T_SELECT[] =
                                   "SELECT ID,NAME,SIGN FROM T_APK;";
    constexpr static Jchar T_DELETE[] =
                                   "DELETE FROM T_APK;";
    constexpr static Jchar T_INSERT[] =
                                   "INSERT INTO T_APK(NAME,SIGN) VALUES(?,?)";

    constexpr static Jchar T_SELECT_WITH_ID[]   =
                                   "SELECT ID,NAME,SIGN FROM T_APK WHERE ID=?;";
    constexpr static Jchar T_SELECT_WITH_NAME[] =
                                   "SELECT ID,NAME,SIGN FROM T_APK WHERE NAME=?;";
    constexpr static Jchar T_SELECT_WITH_SIGN[] =
                                   "SELECT ID,NAME,SIGN FROM T_APK WHERE SIGN=?;";

    constexpr static Jchar T_UPDATE_WITH_ID[]   =
                                   "UPDATE T_APK SET NAME=?,SIGN=? WHERE ID=?;";
    constexpr static Jchar T_UPDATE_WITH_NAME[] =
                                   "UPDATE T_APK SET NAME=?,SIGN=? WHERE NAME=?;";
    constexpr static Jchar T_UPDATE_WITH_SIGN[] =
                                   "UPDATE T_APK SET NAME=?,SIGN=? WHERE SIGN=?;";

    constexpr static Jchar T_DELETE_WITH_ID[]   =
                                   "DELETE FROM T_APK WHERE ID=?;";
    constexpr static Jchar T_DELETE_WITH_NAME[] =
                                   "DELETE FROM T_APK WHERE NAME=?;";
    constexpr static Jchar T_DELETE_WITH_SIGN[] =
                                   "DELETE FROM T_APK WHERE SIGN=?;";

    sqlite3     *mSQL;
    std::string mRootPath;

public:
    APKLocalFile();

    ~APKLocalFile() override;

    void start(const Jchar *path) override;

    Jbool select(std::vector<APKLocalBeanFileCon> &array) override;

    Jbool select(
            APKLocalBeanFileVar cond,
            APKLocalBeanFileCon &value,
            std::vector<APKLocalBeanFileCon> &array
    ) override;

    Jbool insert(APKLocalBeanFileCon &value) override;

    Jbool update(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value, APKLocalBeanFileCon &write) override;

    Jbool remove() override;

    Jbool remove(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value) override;
};

APKLocalFile::APKLocalFile()
        : mSQL{}
          , mRootPath{}
{
}

APKLocalFile::~APKLocalFile()
{
    if (this->mSQL != nullptr)
        sqlite3_close(this->mSQL);
}

void APKLocalFile::start(const Jchar *path)
{
    this->mRootPath.append(path)
            .append(TABLE_SIGN);

    sqlite3_open(this->mRootPath.c_str(), &this->mSQL);
    if (this->mSQL != nullptr)
        sqlite3_exec(this->mSQL, T_CREATE, nullptr, nullptr, nullptr);
}

Jbool APKLocalFile::select(std::vector<APKLocalBeanFileCon> &array)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        array.clear();
        if (sqlite3_prepare_v2(
                this->mSQL,
                T_SELECT,
                strlen(T_SELECT),
                &stmt,
                nullptr
        ) != SQLITE_OK)
            break;

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            array.push_back(
                    APKLocalBeanFileCon().setID(sqlite3_column_int(stmt, 0))
                            .setName(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 1))))
                            .setSign(
                                    reinterpret_cast<Jbyte *>(const_cast<void *>(sqlite3_column_blob(stmt, 2))),
                                    sqlite3_column_bytes(stmt, 2)
                            )
            );
        }

        state = true;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFile::select(
        APKLocalBeanFileVar cond,
        APKLocalBeanFileCon &value,
        std::vector<APKLocalBeanFileCon> &array
)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        array.clear();
        if (cond == APKLocalBeanFileVar::ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_ID,
                    strlen(T_SELECT_WITH_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileVar::FILE_NAME)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_NAME,
                    strlen(T_SELECT_WITH_NAME),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileVar::FILE_SIGN)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_SIGN,
                    strlen(T_SELECT_WITH_SIGN),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else
        {
            break;
        }

        if (cond == APKLocalBeanFileVar::ID)
            sqlite3_bind_int(stmt, 1, value.getID());
        else if (cond == APKLocalBeanFileVar::FILE_NAME)
            sqlite3_bind_text(stmt, 1, value.getName().data(), value.getName().length(), nullptr);
        else
            sqlite3_bind_blob(stmt, 1, value.getSign().data(), value.getSign().size(), nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            array.push_back(
                    APKLocalBeanFileCon().setID(sqlite3_column_int(stmt, 0))
                            .setName(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 1))))
                            .setSign(
                                    reinterpret_cast<Jbyte *>(const_cast<void *>(sqlite3_column_blob(stmt, 2))),
                                    sqlite3_column_bytes(stmt, 2)
                            )
            );
        }

        state = true;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFile::insert(APKLocalBeanFileCon &value)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        if (sqlite3_prepare_v2(
                this->mSQL,
                T_INSERT,
                strlen(T_INSERT),
                &stmt,
                nullptr
        ) != SQLITE_OK)
            break;

        sqlite3_bind_text(stmt, 1, value.getName().data(), value.getName().length(), nullptr);
        sqlite3_bind_blob(stmt, 2, value.getSign().data(), value.getSign().size(), nullptr);

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFile::update(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value, APKLocalBeanFileCon &write)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        if (cond == APKLocalBeanFileVar::FILE_NAME)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_UPDATE_WITH_NAME,
                    strlen(T_UPDATE_WITH_NAME),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileVar::FILE_SIGN)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_UPDATE_WITH_SIGN,
                    strlen(T_UPDATE_WITH_SIGN),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else
        {
            break;
        }

        sqlite3_bind_text(stmt, 1, write.getName().data(), write.getName().length(), nullptr);
        sqlite3_bind_blob(stmt, 2, write.getSign().data(), write.getSign().size(), nullptr);

        if (cond == APKLocalBeanFileVar::FILE_NAME)
            sqlite3_bind_text(stmt, 3, value.getName().data(), value.getName().length(), nullptr);
        else
            sqlite3_bind_blob(stmt, 3, value.getSign().data(), value.getSign().size(), nullptr);

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFile::remove()
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        if (sqlite3_prepare_v2(
                this->mSQL,
                T_DELETE,
                strlen(T_DELETE),
                &stmt,
                nullptr
        ) != SQLITE_OK)
            break;

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFile::remove(APKLocalBeanFileVar cond, APKLocalBeanFileCon &value)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        if (cond == APKLocalBeanFileVar::ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_ID,
                    strlen(T_DELETE_WITH_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileVar::FILE_NAME)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_NAME,
                    strlen(T_DELETE_WITH_NAME),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileVar::FILE_SIGN)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_SIGN,
                    strlen(T_DELETE_WITH_SIGN),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else
        {
            break;
        }

        if (cond == APKLocalBeanFileVar::ID)
            sqlite3_bind_int(stmt, 1, value.getID());
        else if (cond == APKLocalBeanFileVar::FILE_NAME)
            sqlite3_bind_text(stmt, 1, value.getName().data(), value.getName().length(), nullptr);
        else
            sqlite3_bind_blob(stmt, 1, value.getSign().data(), value.getSign().size(), nullptr);

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

}

}

#endif //CSIGNAPK_APKLOCALFILE_HPP
