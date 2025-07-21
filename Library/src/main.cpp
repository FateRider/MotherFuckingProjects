/*--- Library maneger ---*/
#include "sqlite3.h"
#include <iostream>
#include <string>

class dbManager {
  private:
    sqlite3 *db;

  public:
    // DataBase connection
    dbManager(const char *filename) {
        if (sqlite3_open(filename, &db)) throw std::runtime_error("Cannot open database" + std::string(sqlite3_errmsg(db)));

        const char *createQueryselectQuery = R"(
            CREATE TABLE IF NOT EXISTS books (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                author TEXT NOT NULL,
                available INTEGER NOT NULL CHECK(available IN (0,1))
            );
        )";

        char *errMsg = nullptr;

        if (sqlite3_exec(db, createQueryselectQuery, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::string err = errMsg;
            sqlite3_free(errMsg);
            throw std::runtime_error("create  table failed: " + err);
        }
    }

    // Database close
    ~dbManager() {
        if (db) sqlite3_close(db);
    }

    // Add book
    void addbook(const std::string &title, const std::string &author) {

        const char *insertQuery = "INSERT INTO books (title, author, available) VALUES(?, ?, 1)";
        sqlite3_stmt *stmt;

        // prepareing the QueryselectQuery
        if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Add book PREPARE error : " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // process data
        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_TRANSIENT);
        // we are using -1 because : SQLite arranges string length

        // Start the QueryselectQuery
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "-[" << title << "]-> added to database.\n";
        }

        // Release memory
        sqlite3_finalize(stmt);
    }

    // list book
    void listBooks() {
        const char *query = "SELECT title, author, available FROM books";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "list query PREPARE Error :" << sqlite3_errmsg(db) << std::endl;
            return;
        }

        std::cout << "\n--- Books in Library ---\n";

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            std::string author = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            int available = sqlite3_column_int(stmt, 2);

            std::string status = (available == 1) ? "Available" : "Borrowed";

            std::cout << "Title: " << title << "\nAuthor: " << author << "\nStatus: " << status << "\n---\n";
        }

        sqlite3_finalize(stmt);
    }

    // Borrow Book
    void borrowBook(const std::string &title) {

        const char *selectQuery = "SELECT available FROM books WHERE title = ?";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Select prepare failed: " << sqlite3_errmsg(db) << "\n";
            return;
        }

        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int available = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt); // Query done

            if (available == 1) {
                // 2. Güncelle
                const char *updateQuery = "UPDATE books SET available = 0 WHERE title = ?";
                if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr) != SQLITE_OK) {
                    std::cerr << "Update prepare failed: " << sqlite3_errmsg(db) << "\n";
                    return;
                }

                sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

                if (sqlite3_step(stmt) == SQLITE_DONE) {
                    std::cout << "-[" << title << "]-> borrowed.\n";
                } else {
                    std::cerr << "Update failed: " << sqlite3_errmsg(db) << "\n";
                }

                sqlite3_finalize(stmt);
            } else {
                std::cout << "-[" << title << "]-> is already borrowed.\n";
            }
        } else {
            std::cout << "Book not found.\n";
            sqlite3_finalize(stmt);
        }
    }

    // Return Book
    void returnBook(const std::string &title) {
        const char *selectQuery = "SELECT available FROM books WHERE title = ?";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Select prepare failed: " << sqlite3_errmsg(db) << "\n";
            return;
        }

        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int available = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);

            if (available == 0) {
                const char *updateQuery = "UPDATE books SET available = 1 WHERE title = ?";
                if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr) != SQLITE_OK) {
                    std::cerr << "Update prepare failed: " << sqlite3_errmsg(db) << "\n";
                    return;
                }

                sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

                if (sqlite3_step(stmt) == SQLITE_DONE) {
                    std::cout << "-[" << title << "]-> Returned.\n";
                } else {
                    std::cerr << "Update failed: " << sqlite3_errmsg(db) << "\n";
                }

                sqlite3_finalize(stmt);
            } else {
                std::cout << "-[" << title << "]-> Was not borrowed.\n";
            }
        } else {
            std::cout << "Book not found. \n";
            sqlite3_finalize(stmt);
        }
    }

    // Delete book
    void deleteBook(const std::string &title) {
        const char *deleteQuery = "DELETE FROM books WHERE title = ? COLLATE NOCASE";
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(db, deleteQuery, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Delete prepare failed: " << sqlite3_errmsg(db) << "\n";
            return;
        }

        sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            if (sqlite3_changes(db) > 0) {
                std::cout << "-[" << title << "]-> deleted from library.\n";
            } else {
                std::cout << "book not found to delete. \n";
            }
        } else {
            std::cerr << "Delete failed: " << sqlite3_errmsg(db) << "\n";
        }

        sqlite3_finalize(stmt);
    }
};

int main() {
    try {

        dbManager db("Library.db");
        int choice;

        while (true) {
            std::cout << "\n--- Library Menu ---\n";
            std::cout << "1. Add Book\n";
            std::cout << "2. List Books\n";
            std::cout << "3. Borrow Book\n";
            std::cout << "4. Return Book\n";
            std::cout << "5. Delete book \n";
            std::cout << "0. Exit\n";
            std::cout << "Choice: ";
            std::cin >> choice;

            std::cin.ignore();

            if (choice == 0) break;

            std::string author, title;

            switch (choice) {
            case 1:
                std::cout << "Title : ";
                std::getline(std::cin, title);
                std::cout << "Author : ";
                std::getline(std::cin, author);
                db.addbook(title, author);
                break;

            case 2:
                db.listBooks();
                break;

            case 3:
                std::cout << "Book title to borrow : ";
                std::getline(std::cin, title);
                db.borrowBook(title);
                break;

            case 4:
                std::cout << "Book title to return : ";
                std::getline(std::cin, title);
                db.returnBook(title);
                break;

            case 5:
                std::cout << "Book title to delete : ";
                std::getline(std::cin, title);
                db.deleteBook(title);
                break;

            default:
                std::cout << "Invalid choice!\n";
            }
        }
    } catch (const std::exception &e) { std::cerr << "Fatal :" << e.what() << std::endl; }

    std::cout << "exiting ... \n";
    return 0;
}