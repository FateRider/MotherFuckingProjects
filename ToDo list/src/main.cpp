/*-- ToDo List --*/
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

class listelement {
  public:
    string header;
    string description;
    bool status;
};

class ToDoList {
  public:
    string listName;

    vector<listelement> listdb;
};

void add(ToDoList &todolist, string x, string y, bool z) {

    listelement newelement;

    newelement.header = x;
    newelement.description = y;
    newelement.status = z;

    todolist.listdb.push_back(newelement);
}

void adddb(vector<ToDoList> &dbentry) {

    json TodolistDb;

    for (const auto &todolist : dbentry) {
        for (const auto &item : todolist.listdb) {
            TodolistDb[todolist.listName].push_back({{"header", item.header}, {"description", item.description}, {"status", item.status}});
        }
    }

    ofstream TodoDataBase("todolistdb.json");
    TodoDataBase << TodolistDb.dump(4);
}

vector<ToDoList> loaddb() {
    vector<ToDoList> db;
    ifstream TodoDataBase("todolistdb.json");
    if (!TodoDataBase.is_open()) return db;

    json TodolistDb;
    TodoDataBase >> TodolistDb;

    for (auto &[listName, elements] : TodolistDb.items()) {
        ToDoList list;
        list.listName = listName;
        for (auto &el : elements) {
            listelement item;
            item.header = el["header"];
            item.description = el["description"];
            item.status = el["status"];
            list.listdb.push_back(item);
        }
        db.push_back(list);
    }

    return db;
}

int main() {

    vector<ToDoList> db = loaddb();
    string filename;
    string entry1, entry2, statusInput;
    bool entry3;

    if (db.empty()) {

        cout << "Please enter new TodoList name \n";
        getline(cin, filename);

        ToDoList userlist;
        userlist.listName = filename;

        for (int query = 3; query > 0;) {

            cout << "Plase enter new list element header \n";
            getline(cin, entry1);

            cout << "Plase enter new list element description \n";
            getline(cin, entry2);

            cout << "Plase enter new list status \n";
            cin >> statusInput;

            for (char &c : statusInput)
                c = tolower(c);

            if (statusInput == "true" || statusInput == "1" || statusInput == "yes") {
                entry3 = true;
            } else {
                entry3 = false;
            }

            cout << "Again creat new todo ? \n";
            cout << "yes = 1 \n";
            cout << "no = 0 \n";

            cin >> query;

            cin.clear();
            cin.ignore(10000, '\n');

            add(userlist, entry1, entry2, entry3);
        }

        db.push_back(userlist);
        adddb(db);

    } else {

        cout << "\n -- Mevcut listeler -- \n";
        int i = 1;
        for (auto lists : db) {
            cout << i << ' ' << lists.listName << endl;
            i++;
        }

        cout << "\n Please enter the number of the list you want to edit. \n";
        cout << "Enter 0 to create a new list. \n";

        int select;
        cin >> select;

        cin.clear();
        cin.ignore(10000, '\n');

        if (select == 0) {
            cout << "Please enter new TodoList name \n";
            getline(cin, filename);

            ToDoList userlist;
            userlist.listName = filename;

            for (int query = 3; query > 0;) {

                cout << "Plase enter new list element header \n";
                getline(cin, entry1);

                cout << "Plase enter new list element description \n";
                getline(cin, entry2);

                cout << "Plase enter new list status \n";
                cin >> statusInput;

                for (char &c : statusInput)
                    c = tolower(c);

                if (statusInput == "true" || statusInput == "1" || statusInput == "yes") {
                    entry3 = true;
                } else {
                    entry3 = false;
                }

                cout << "Again creat new todo ? \n";
                cout << "yes = 1 \n";
                cout << "no = 0 \n";

                cin >> query;

                cin.clear();
                cin.ignore(10000, '\n');

                add(userlist, entry1, entry2, entry3);
            }

            db.push_back(userlist);
            adddb(db);

        } else {

            ToDoList listch = db[select - 1];
            for (auto line : listch.listdb) {
                cout << line.header << " | " << line.description << " | " << line.status << endl;
            }

            cout << "\nWhich item do you want to edit? (1 to " << listch.listdb.size() << ", 0 to cancel)\n";
            int editIndex;
            cin >> editIndex;

            cin.clear();
            cin.ignore(10000, '\n');

            if (editIndex > 0 && editIndex <= listch.listdb.size()) {
                listelement &elem = listch.listdb[editIndex - 1];

                cout << "\nEditing item: " << elem.header << " | " << elem.description << " | " << (elem.status ? "Done" : "Not done") << endl;

                cout << "New header (leave empty to keep current): ";
                string newHeader;
                getline(cin, newHeader);
                if (!newHeader.empty()) elem.header = newHeader;

                cout << "New description (leave empty to keep current): ";
                string newDescription;
                getline(cin, newDescription);
                if (!newDescription.empty()) elem.description = newDescription;

                cout << "New status (true/false or 1/0, leave empty to keep current): ";
                string newStatus;
                getline(cin, newStatus);
                if (!newStatus.empty()) {
                    for (char &c : newStatus)
                        c = tolower(c);
                    elem.status = (newStatus == "true" || newStatus == "1" || newStatus == "yes");
                }

                db[select - 1] = listch;
                adddb(db);

                cout << "Item updated and database saved.\n";

            } else {
                cout << "Edit cancelled.\n";
            }
        }
        // hmm
    }

    return 0;
}