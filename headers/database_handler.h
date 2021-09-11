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

        if (!check_login_table()) {
            create_login_table();
        }

        if (!check_settings_table()) {
            create_settings_table();
        }

    }

    bool check_login_table() {
        bool exists = false;
        char sql[] = "SELECT * FROM LOGIN";
        struct sqlite3_stmt *selectstmt;
        int result = sqlite3_prepare_v2(db, sql, -1, &selectstmt, nullptr);
        if (result == SQLITE_OK) {
            sqlite3_free(selectstmt);
            exists = true;
        }
        return exists;
    }

    bool check_settings_table() {
        bool exists = false;
        char sql[] = "SELECT * FROM SETTINGS";
        struct sqlite3_stmt *selectstmt;
        int result = sqlite3_prepare(db, sql, -1, &selectstmt, nullptr);
        if (result == SQLITE_OK) {
            sqlite3_free(selectstmt);
            exists = true;
        }
        return exists;
    }

    void create_settings_table() {
        char create_table[] = "CREATE TABLE SETTINGS(host_ip string, udp_port int)";
        rc = sqlite3_exec(db, create_table, nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
        insert_default_setting_values();
    }

    void insert_default_setting_values() {
        char default_values[] = "INSERT INTO SETTINGS VALUES('127.0.0.1', 1235)";
        rc = sqlite3_exec(db, default_values, nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
    }

    void create_login_table() {
        char create_table[] = "CREATE TABLE LOGIN(phone_number text, password text)";
        rc = sqlite3_exec(db, create_table, nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
    }

    bool login(const char* phone_number, const char* password) {
        char sql[] = "SELECT * FROM LOGIN WHERE phone_number = ? and password = ?";
        struct sqlite3_stmt *selectstmt;
        if (sqlite3_prepare_v2(db, sql, -1, &selectstmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(selectstmt, 1, phone_number, -1, nullptr);
            sqlite3_bind_text(selectstmt, 2, password, -1, nullptr);
            if (sqlite3_step(selectstmt) == SQLITE_ROW) {
                // record found
                sqlite3_finalize(selectstmt);
                return true;
            }
        }
        return false;
    }


    void register_user(const std::string& phone_number, const std::string& password) {
        std::string registration = "INSERT INTO LOGIN VALUES(" + phone_number + ", " + password + ")";
        rc = sqlite3_exec(db, registration.c_str(), nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
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
        char create_table[] = "CREATE TABLE CONTACTS(username text, phone_number text)";
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

    std::string get_host_ip() {
        std::string host_ip;
        char query[] = "SELECT host_ip FROM SETTINGS";
        struct sqlite3_stmt * selectStmt;
        if (sqlite3_prepare(db, query, -1, &selectStmt, nullptr) == SQLITE_OK) {
            int ctotal = sqlite3_column_count(selectStmt); // Count the Number of Columns in the Table
            int res;
            while (true) {
                res = sqlite3_step(selectStmt); // Execute SQL Statement.
                if (res == SQLITE_ROW) {
                    //sqlite3_finalize(selectStmt);
                    for (int i = 0; i < ctotal; i++)  // Loop times the number of columns in the table
                    {
                        std::string s = (char *) sqlite3_column_text(selectStmt,
                                                                     i);  // Read each Column in the row.
                        // print or format the output as you want
                        host_ip += s;
                    }
                }

                if (res == SQLITE_DONE || res == SQLITE_ERROR) {
                    break;
                }
            }
        }
        sqlite3_finalize(selectStmt);
        return host_ip;
    }

    int get_default_port() {
        int port;
        char query[] = "SELECT udp_port FROM SETTINGS";
        struct sqlite3_stmt * selectStmt;
        if (sqlite3_prepare(db, query, -1, &selectStmt, nullptr) == SQLITE_OK) {
            int ctotal = sqlite3_column_count(selectStmt); // Count the Number of Columns in the Table
            int res;
            while (true) {
                res = sqlite3_step(selectStmt); // Execute SQL Statement.
                if (res == SQLITE_ROW) {
                    //sqlite3_finalize(selectStmt);
                    for (int i = 0; i < ctotal; i++)  // Loop times the number of columns in the table
                    {
                        int s = (int) sqlite3_column_int(selectStmt, i);  // Read each Column in the row.
                        // print or format the output as you want
                        port = s;
                    }
                }

                if (res == SQLITE_DONE || res == SQLITE_ERROR) {
                    break;
                }
            }
        }
        sqlite3_finalize(selectStmt);
        return port;
    }

    void update_host_ip(const std::string& host_ip) {
        std::string update_sql = "UPDATE SETTINGS SET host_ip = " + host_ip + "WHERE rowid = 1";
        rc = sqlite3_exec(db, update_sql.c_str(), nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
    }

    void update_udp_port(const std::string& udp_port) {
        std::string update_port = "UPDATE SETTINGS SET udp_port = " + udp_port + " WHERE rowid = 1";
        rc = sqlite3_exec(db, udp_port.c_str(), nullptr, nullptr, &zErrMsg);
        sqlite3_free(zErrMsg);
    }

    std::string get_phone_number() {
        std::string phone_number;

        char query[] = "SELECT phone_number FROM LOGIN LIMIT 1";
        struct sqlite3_stmt *selectStmt;
        if (sqlite3_prepare(db, query, -1, &selectStmt, nullptr) == SQLITE_OK) {
            int ctotal = sqlite3_column_count(selectStmt); // Count the Number of Columns in the Table
            int res;
            while (true) {
                res = sqlite3_step(selectStmt); // Execute SQL Statement.
                if (res == SQLITE_ROW) {
                    //sqlite3_finalize(selectStmt);
                    for (int i = 0; i < ctotal; i++)  // Loop times the number of columns in the table
                    {
                        std::string s = (char *) sqlite3_column_text(selectStmt,
                                                                     i);  // Read each Column in the row.
                        // print or format the output as you want
                        phone_number += s;
                    }
                }

                if (res == SQLITE_DONE || res == SQLITE_ERROR) {
                    break;
                }
            }
        }
        sqlite3_finalize(selectStmt);
        return phone_number;
    }



private:
    int rc{};
    sqlite3 *db{};
    char *zErrMsg = nullptr;
};

#endif //VOIP_CLIENT3_DATABASE_HANDLER_H
