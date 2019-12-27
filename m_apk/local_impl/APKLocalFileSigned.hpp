#pragma once

#ifndef CSIGNAPK_APKLOCALFILESIGNED_HPP
#define CSIGNAPK_APKLOCALFILESIGNED_HPP

#include "bean/APKLocalBeanFileSignedVar.hpp"
#include "bean/APKLocalBeanFileSignedCon.hpp"

#include <sqlite3.h>

namespace m
{

inline namespace apk
{

class APKLocalFileSigned
        : public APKLocalActionI<APKLocalBeanFileSignedVar, APKLocalBeanFileSignedCon>
{
private:
    constexpr static Jchar TABLE_SIGN[] = "/36FF4C01704BB0815D4110EAE8AA359C";

    constexpr static Jchar T_CREATE[] =
                                   "CREATE TABLE T_APK_SIGNED("
                                   "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "FILE_ID INTEGER,"
                                   "NAME TEXT,"
                                   "SHA1 TEXT,"
                                   "SHA256 TEXT"
                                   ");";

    constexpr static Jchar T_SELECT[] =
                                   "SELECT ID,FILE_ID,NAME,SHA1,SHA256 FROM T_APK_SIGNED;";
    constexpr static Jchar T_DELETE[] =
                                   "DELETE FROM T_APK_SIGNED;";
    constexpr static Jchar T_INSERT[] =
                                   "INSERT INTO T_APK_SIGNED(FILE_ID,NAME,SHA1,SHA256) VALUES(?,?,?,?);";

    constexpr static Jchar T_SELECT_WITH_ID[]      =
                                   "SELECT ID,FILE_ID,NAME,SHA1,SHA256 FROM T_APK_SIGNED WHERE ID=?;";
    constexpr static Jchar T_SELECT_WITH_FILE_ID[] =
                                   "SELECT ID,FILE_ID,NAME,SHA1,SHA256 FROM T_APK_SIGNED WHERE FILE_ID=?;";
    constexpr static Jchar T_SELECT_WITH_NAME[]    =
                                   "SELECT ID,FILE_ID,NAME,SHA1,SHA256 FROM T_APK_SIGNED WHERE NAME=?;";
    constexpr static Jchar T_SELECT_WITH_SHA1[]    =
                                   "SELECT ID,FILE_ID,NAME,SHA1,SHA256 FROM T_APK_SIGNED WHERE SHA1=?;";
    constexpr static Jchar T_SELECT_WITH_SHA256[]  =
                                   "SELECT ID,FILE_ID,NAME,SHA1,SHA256 FROM T_APK_SIGNED WHERE SHA256=?;";

    constexpr static Jchar T_UPDATE_WITH_ID[]      =
                                   "UPDATE T_APK_SIGNED SET FILE_ID=?,NAME=?,SHA1=?,SHA256=? WHERE ID=?;";
    constexpr static Jchar T_UPDATE_WITH_FILE_ID[] =
                                   "UPDATE T_APK_SIGNED SET FILE_ID=?,NAME=?,SHA1=?,SHA256=? WHERE FILE_ID=?;";
    constexpr static Jchar T_UPDATE_WITH_NAME[]    =
                                   "UPDATE T_APK_SIGNED SET FILE_ID=?,NAME=?,SHA1=?,SHA256=? WHERE NAME=?;";
    constexpr static Jchar T_UPDATE_WITH_SHA1[]    =
                                   "UPDATE T_APK_SIGNED SET FILE_ID=?,NAME=?,SHA1=?,SHA256=? WHERE SHA1=?;";
    constexpr static Jchar T_UPDATE_WITH_SHA256[]  =
                                   "UPDATE T_APK_SIGNED SET FILE_ID=?,NAME=?,SHA1=?,SHA256=? WHERE SHA256=?;";

    constexpr static Jchar T_DELETE_WITH_ID[]        =
                                   "DELETE FROM T_APK_SIGNED WHERE ID=?;";
    constexpr static Jchar T_DELETE_WITH_FILE_ID[]   =
                                   "DELETE FROM T_APK_SIGNED WHERE FILE_ID=?;";
    constexpr static Jchar T_DELETE_WITH_FILE_NAME[] =
                                   "DELETE FROM T_APK_SIGNED WHERE NAME=?;";
    constexpr static Jchar T_DELETE_WITH_SHA1[]      =
                                   "DELETE FROM T_APK_SIGNED WHERE SHA1=?;";
    constexpr static Jchar T_DELETE_WITH_SHA256[]    =
                                   "DELETE FROM T_APK_SIGNED WHERE SHA256=?;";

    sqlite3     *mSQL;
    std::string mRootPath;

public:
    APKLocalFileSigned();

    ~APKLocalFileSigned() override;

    void start(const Jchar *path) override;

    Jbool select(std::vector<APKLocalBeanFileSignedCon> &array) override;

    Jbool select(
            APKLocalBeanFileSignedVar cond,
            APKLocalBeanFileSignedCon &value,
            std::vector<APKLocalBeanFileSignedCon> &array
    ) override;

    Jbool insert(APKLocalBeanFileSignedCon &value) override;

    Jbool update(
            APKLocalBeanFileSignedVar cond,
            APKLocalBeanFileSignedCon &value,
            APKLocalBeanFileSignedCon &write
    ) override;

    Jbool remove() override;

    Jbool remove(APKLocalBeanFileSignedVar cond, APKLocalBeanFileSignedCon &value) override;
};

APKLocalFileSigned::APKLocalFileSigned()
        : mSQL{}
          , mRootPath{}
{
}

APKLocalFileSigned::~APKLocalFileSigned()
{
    if (this->mSQL != nullptr)
        sqlite3_close(this->mSQL);
}

void APKLocalFileSigned::start(const Jchar *path)
{
    this->mRootPath.append(path)
            .append(TABLE_SIGN);

    sqlite3_open(this->mRootPath.c_str(), &this->mSQL);
    if (this->mSQL != nullptr)
        sqlite3_exec(this->mSQL, T_CREATE, nullptr, nullptr, nullptr);
}

Jbool APKLocalFileSigned::select(std::vector<APKLocalBeanFileSignedCon> &array)
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
                    APKLocalBeanFileSignedCon().setID(sqlite3_column_int(stmt, 0))
                            .setFileID(sqlite3_column_int(stmt, 1))
                            .setName(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 2))))
                            .setSHA1(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 3))))
                            .setSHA256(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 4))))
            );
        }

        state = true;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFileSigned::select(
        APKLocalBeanFileSignedVar cond,
        APKLocalBeanFileSignedCon &value,
        std::vector<APKLocalBeanFileSignedCon> &array
)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        array.clear();
        if (cond == APKLocalBeanFileSignedVar::ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_ID,
                    strlen(T_SELECT_WITH_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_FILE_ID,
                    strlen(T_SELECT_WITH_FILE_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::NAME)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_NAME,
                    strlen(T_SELECT_WITH_NAME),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_1)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_SHA1,
                    strlen(T_SELECT_WITH_SHA1),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_256)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_SELECT_WITH_SHA256,
                    strlen(T_SELECT_WITH_SHA256),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else
        {
            break;
        }

        if (cond == APKLocalBeanFileSignedVar::ID)
            sqlite3_bind_int(stmt, 1, value.getID());
        else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
            sqlite3_bind_int(stmt, 1, value.getFileID());
        else if (cond == APKLocalBeanFileSignedVar::NAME)
            sqlite3_bind_text(stmt, 1, value.getName().data(), value.getName().length(), nullptr);
        else if (cond == APKLocalBeanFileSignedVar::SHA_1)
            sqlite3_bind_text(stmt, 1, value.getSHA1().data(), value.getSHA1().length(), nullptr);
        else
            sqlite3_bind_text(stmt, 1, value.getSHA256().data(), value.getSHA256().length(), nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            array.push_back(
                    APKLocalBeanFileSignedCon().setID(sqlite3_column_int(stmt, 0))
                            .setFileID(sqlite3_column_int(stmt, 1))
                            .setName(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 2))))
                            .setSHA1(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 3))))
                            .setSHA256(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(sqlite3_column_text(stmt, 4))))
            );
        }

        state = true;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFileSigned::insert(APKLocalBeanFileSignedCon &value)
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

        sqlite3_bind_int(stmt, 1, value.getFileID());
        sqlite3_bind_text(stmt, 2, value.getName().data(), value.getName().length(), nullptr);
        sqlite3_bind_text(stmt, 3, value.getSHA1().data(), value.getSHA1().length(), nullptr);
        sqlite3_bind_text(stmt, 4, value.getSHA256().data(), value.getSHA256().length(), nullptr);

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFileSigned::update(
        APKLocalBeanFileSignedVar cond,
        APKLocalBeanFileSignedCon &value,
        APKLocalBeanFileSignedCon &write
)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        if (cond == APKLocalBeanFileSignedVar::FILE_ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_UPDATE_WITH_FILE_ID,
                    strlen(T_UPDATE_WITH_FILE_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::NAME)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_UPDATE_WITH_NAME,
                    strlen(T_UPDATE_WITH_NAME),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_1)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_UPDATE_WITH_SHA1,
                    strlen(T_UPDATE_WITH_SHA1),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_256)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_UPDATE_WITH_SHA256,
                    strlen(T_UPDATE_WITH_SHA256),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else
        {
            break;
        }

        sqlite3_bind_int(stmt, 1, write.getFileID());
        sqlite3_bind_text(stmt, 2, write.getName().data(), write.getName().length(), nullptr);
        sqlite3_bind_text(stmt, 3, write.getSHA1().data(), write.getSHA256().length(), nullptr);
        sqlite3_bind_text(stmt, 4, write.getSHA256().data(), write.getSHA256().length(), nullptr);

        if (cond == APKLocalBeanFileSignedVar::FILE_ID)
            sqlite3_bind_int(stmt, 5, value.getFileID());
        else if (cond == APKLocalBeanFileSignedVar::NAME)
            sqlite3_bind_text(stmt, 5, value.getName().data(), value.getName().length(), nullptr);
        else if (cond == APKLocalBeanFileSignedVar::SHA_1)
            sqlite3_bind_text(stmt, 5, value.getSHA1().data(), value.getSHA1().length(), nullptr);
        else
            sqlite3_bind_text(stmt, 5, value.getSHA256().data(), value.getSHA256().length(), nullptr);

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

Jbool APKLocalFileSigned::remove()
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

Jbool APKLocalFileSigned::remove(APKLocalBeanFileSignedVar cond, APKLocalBeanFileSignedCon &value)
{
    Jbool        state = false;
    sqlite3_stmt *stmt = nullptr;

    do
    {
        if (cond == APKLocalBeanFileSignedVar::ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_ID,
                    strlen(T_DELETE_WITH_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_FILE_ID,
                    strlen(T_DELETE_WITH_FILE_ID),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::NAME)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_FILE_NAME,
                    strlen(T_DELETE_WITH_FILE_NAME),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_1)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_SHA1,
                    strlen(T_DELETE_WITH_SHA1),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else if (cond == APKLocalBeanFileSignedVar::SHA_256)
        {
            if (sqlite3_prepare_v2(
                    this->mSQL,
                    T_DELETE_WITH_SHA256,
                    strlen(T_DELETE_WITH_SHA256),
                    &stmt,
                    nullptr
            ) != SQLITE_OK)
                break;
        } else
        {
            break;
        }

        if (cond == APKLocalBeanFileSignedVar::ID)
            sqlite3_bind_int(stmt, 1, value.getID());
        else if (cond == APKLocalBeanFileSignedVar::FILE_ID)
            sqlite3_bind_int(stmt, 1, value.getFileID());
        else if (cond == APKLocalBeanFileSignedVar::NAME)
            sqlite3_bind_text(stmt, 1, value.getName().data(), value.getName().length(), nullptr);
        else if (cond == APKLocalBeanFileSignedVar::SHA_1)
            sqlite3_bind_text(stmt, 1, value.getSHA1().data(), value.getSHA1().length(), nullptr);
        else
            sqlite3_bind_text(stmt, 1, value.getSHA256().data(), value.getSHA256().length(), nullptr);

        state = sqlite3_step(stmt) == SQLITE_DONE;
    } while (false);

    if (stmt != nullptr)
        sqlite3_finalize(stmt);
    return state;
}

}

}

#endif //CSIGNAPK_APKLOCALFILESIGNED_HPP
