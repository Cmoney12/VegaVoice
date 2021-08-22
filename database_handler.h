//
// Created by corey on 8/21/21.
//

#ifndef VOIP_CLIENT3_DATABASE_HANDLER_H
#define VOIP_CLIENT3_DATABASE_HANDLER_H

#include <sqlite3.h>

class database_handler {
public:
    explicit database_handler(const char* db_path) {
        sqlite3_open(db_path, &db);
    }

    ~database_handler() {
        sqlite3_close(db);
    }

    void check_tables() {
        if (!check_contacts_table()) {
            create_contacts_table();
        }
    }

    bool check_contacts_table() {
        bool exists = false;
        char sql[] = "SELECT * FROM CONTACTS";
        struct sqlite3_stmt *selectstmt;
        int result = sqlite3_prepare_v2(db, sql, -1, &selectstmt, nullptr);
        if (result == SQLITE_OK) {
            sqlite3_free(selectstmt);
            exists = true;
        }
        return exists;
    }

    void create_contacts_table() {
        char create_table[] = "CREATE TABLE CONTACTS(username text, phone_number text);";
        rc = sqlite3_exec(db, create_table, nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
    }

    void create_contact(const char* contact_name, const char* phone_number) {
        sqlite3_stmt *insert_stmt = nullptr;
        char insert_contact[] = "INSERT INTO CONTACTS VALUES(?, ?)";

        if (sqlite3_prepare_v2(db, insert_contact, -1, &insert_stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(insert_stmt, 1, contact_name, -1, nullptr);
            sqlite3_bind_text(insert_stmt, 2, phone_number, -1, nullptr);
            rc = sqlite3_step(insert_stmt);
            sqlite3_exec(db, "COMMIT TRANSACTION", nullptr, nullptr, &zErrMsg);
            sqlite3_finalize(insert_stmt);
            sqlite3_free(zErrMsg);
        }
    }



private:
    int rc{};
    sqlite3 *db{};
    char *zErrMsg = nullptr;
};

#endif //VOIP_CLIENT3_DATABASE_HANDLER_H
