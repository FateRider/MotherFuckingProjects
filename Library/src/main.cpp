#include <iostream>
#include <string>
#include <vector>
class book {
    public:
    std::string title , author;
    bool isBorrowed;

    book(const std::string t,const std::string a): title(t), author(a), isBorrowed(0) {};

    //borrow
    void borrow() {
        if(!isBorrowed) {
            isBorrowed = true;
            std::cout << "-[" << title << "]->" << "Has been borrowed\\ \n";
        }else {
            std::cout << "-[" << title << "]->" << "is already borrowed\\ \n";
        }
    };
    
    //Givebeback
    void giveback(){
        if(isBorrowed) {
            isBorrowed = false;
            std::cout << "-[" << title << "]->" << "has been returned\\ \n";
        }else {
            std::cout << "-[" << title << "]->" << "was not borrowed\\ \n";
        }
    };

};


std::ostream &operator<<(std::ostream &bookprint, const book &book){
    bookprint << "Title :" << book.title << std::endl
    << "Author :" << book.author << std::endl
    << "status :" << (book.isBorrowed ? "Borrowed" : "Available" ) << std::endl;
    return bookprint;
}

class Library {
    private:

    std::vector<book*> books;

    public:

    void addBook(book* book) {
        books.push_back(book);
        std::cout << "-[" << book->title << "]->" << "added to library\\ \n";
    }

    void listbooks() const {

        std::cout << "\n--- Library envanter ---\n";

        for(const auto &a : books) {
            std::cout << " -" << *a << std::endl;
        }
        std::cout << "-- Book not found! --\n";
    }

    void borrowBook(const std::string &title){

        for(auto &book : books){
            if(book->title==title) {
                book->borrow();
                return;
            }
        }
        std::cout << "Book not found.\n";

    }
    
    void returnBook(const std::string &title) {
        
        for(auto &book : books) {
            if(book->title == title) {
                book->giveback();
                return;
            }
        }
        std::cout << "-- Book not found! --\n";
    }

    ~Library() {
        for (auto& book : books) {
            delete book;
        }
    }

};

int main() {
    Library lib;
    int choice;

    while (true){
        std::cout << "\n--- Library Menu ---\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. List Books\n";
        std::cout << "3. Borrow Book\n";
        std::cout << "4. Return Book\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        std::cin.ignore();
        
        if(choice == 0) {
            break;
        }

        std::string author,title;

        switch (choice) {
            case 1:
                std::cout << "Title : ";
                std::getline(std::cin, title);
                std::cout << "Author : ";
                std::getline(std::cin, author);
                lib.addBook(new book(title, author));
            break;

            case 2:
                lib.listbooks();
            break;

            case 3:
                std::cout << "Book title to borrow : ";
                std::getline(std::cin, title);
                lib.borrowBook(title);
            break;

            case 4:
                std::cout << "Book title to return : ";
                std::getline(std::cin, title);
                lib.returnBook(title);
            break;

            default:
            std::cout << "Invalid choice!\n";
            
        }
    }

    std::cout << "exiting ... \n";
    return 0;
}