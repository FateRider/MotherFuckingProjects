/*--- Library maneger ---*/
#include <iostream>
#include <mariadb/mysql.h>
#include <string>

class dbManager {
  private:
    MYSQL *conn;

  public:
    // DataBase connection
    dbManager(const char *hostname, const char *username, const char *password, const char *db) {
        conn = mysql_init(nullptr);
        if (!conn) { throw std::runtime_error("MySql İnit fail!"); }

        if (!mysql_real_connect(conn, hostname, username, password, db, 0, NULL, 0)) {
            std::string error = mysql_error(conn);
            mysql_close(conn);
            conn = nullptr;
            throw std::runtime_error("Connection error :" + error);
        }
    }

    // Database close
    ~dbManager() {
        if (conn) mysql_close(conn);
    }

    // Add book
    void addbook(const std::string &title, const std::string &author) {
        std::string query = "INSERT INTO books (title, author, available) VALUES ('" + title + "', '" + author + "', true)";
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "Add book error : " << mysql_error(conn) << std::endl;
            return;
        } else {
            std::cout << "-[" << title << "]->" << " added to database.\n";
        }
    }

    // list book
    void listBooks() {
        const char *query = "SELECT title, author, available FROM books";
        if (mysql_query(conn, query)) {
            std::cerr << "list query Error :" << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (!result) {
            std::cerr << "Result Error :" << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_ROW row;
        std::cout << "\n--- Books in Library ---\n";
        while ((row = mysql_fetch_row(result))) {
            std::string status = (std::string(row[2]) == "1") ? "available" : "Borrowed";
            std::cout << "Title: " << row[0] << "\nAuthor: " << row[1] << "\n---\n";
        }

        mysql_free_result(result);
    }

    // Borrow Book
    void borrowBook(const std::string &title) {
        std::string bookQuery = "SELECT available FROM books WHERE title='" + title + "'";
        if (mysql_query(conn, bookQuery.c_str())) {
            std::cerr << "Borrow Error :" << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (!result) {
            std::cerr << "Result error: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            if (std::string(row[0]) == "1") {
                std::string updateQuery = "UPDATE books SET available=false WHERE title='" + title + "'";
                if (mysql_query(conn, updateQuery.c_str())) {
                    std::cerr << "Update Error :" << mysql_error(conn) << std::endl;
                    return;
                } else {
                    std::cout << "-[" << title << "]-> borrowed.\n";
                }
            }

        } else {
            std::cout << "Book not found \n";
        }

        mysql_free_result(result);
    }

    // Return Book
    void returnBook(const std::string &title) {
        std::string bookQuery = "SELECT available FROM books WHERE title='" + title + "'";
        if (mysql_query(conn, bookQuery.c_str())) {
            std::cerr << "Return Error :" << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_RES *result = mysql_store_result(conn);
        if (!result) {
            std::cerr << "Result error: " << mysql_error(conn) << std::endl;
            return;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row) {
            if (std::string(row[0]) == "0") {
                std::string updateQuery = "UPDATE books SET available=true WHERE title='" + title + "'";
                if (mysql_query(conn, updateQuery.c_str())) {
                    std::cerr << "Update Queary Error :" << mysql_error(conn);
                    return;
                } else {
                    std::cout << "-[" << title << "]-> Returned.\n";
                }
            } else {
                std::cout << "-[" << title << "]-> Was not borrowed.\n";
            }

        } else {
            std::cout << "Book not found \n";
        }

        mysql_free_result(result);
    }
};

int main() {
    try {

        dbManager db("localhost", "root", "12308363602", "Library");
        int choice;

        while (true) {
            std::cout << "\n--- Library Menu ---\n";
            std::cout << "1. Add Book\n";
            std::cout << "2. List Books\n";
            std::cout << "3. Borrow Book\n";
            std::cout << "4. Return Book\n";
            std::cout << "0. Exit\n";
            std::cout << "Choice: ";
            std::cin >> choice;

            std::cin.ignore();

            if (choice == 0) { break; }

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

            default:
                std::cout << "Invalid choice!\n";
            }
        }
    } catch (const std::exception &e) { std::cerr << "Fatal :" << e.what() << std::endl; }

    std::cout << "exiting ... \n";
    return 0;
}