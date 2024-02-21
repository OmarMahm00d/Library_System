/*
test cases:
User 			->done
book			->done
Library_system	->not started
*/

#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

//global variables
const int  MAX = 100;

struct User{
	int id {0};
	string name {""};
	int total_borrowing {0};//0-10
	int books_ids[MAX] {0};
	int len = 0;
	
	User(){}
	User(const int& ID, const string nm) : id(ID), name(nm) {}
	
	bool borrow_book(const int& bk_id){
		if(total_borrowing == 10)//IF the user try to exceed the maximum number of books that he can borrow
			return 0;
		//add book id to list
		books_ids[len++] = bk_id;
		total_borrowing++;
		return 1;
	}
	
	bool return_book(const int& bk_id){
		int i = 0;
		//get book id index from the list
		for(;i<len;i++){
			if(books_ids[i] == bk_id)
				break;
		}
		if(i == len)//the id is not correct
			return 0;
		for(;i<len-1;i++){//shift left from the id index
			books_ids[i] = books_ids[i+1];
		}
		books_ids[--len] = 0;
		total_borrowing--;
		return 1;
	}
	
	void print(){
		cout << "user " << name << " id " << id << " borrowed books ids:";
		if(len == 0)//if the user has not borrowed books yet
			cout << " N/A";
		else
			//print books id	
			for(int i=0;i<len;i++){
				cout << " " << books_ids[i];
			}
			cout << endl;
	}
};

struct Book{
	int id {0};
	string name {""};
	int quantity {0};
	int total_borrowed {0};
	int *user_ids{nullptr};
	string *user_names{nullptr};
	int len = 0;
	
	/* creating and deleting our dynamic memory */
	Book(){}
	Book(const int& ID, const string n, const int& q) : id(ID), name(n), quantity(q) {
		user_ids = new int[q] {0};
		user_names = new string[q];
	}
	~Book(){
		delete[] user_ids;
		delete[] user_names;
		user_ids = nullptr;
		user_names = nullptr;
	}
	/* avoiding shallow copy */
	Book(const Book& other) : id(other.id), name(other.name), quantity(other.quantity){
		const int&size = other.quantity;
		user_ids = new int[size];
		for(int i=0;i<size;i++)
			user_ids[i] = other.user_ids[i];
		user_names = new string[size];
		for(int i=0;i<size;i++)
			user_names[i] = other.user_names[i];
	}
	void operator=(const Book& other){
		id = other.id;
		name = other.name;
		quantity = other.quantity;
		const int&size = other.quantity;
		user_ids = new int[size];
		for(int i=0;i<size;i++)
			user_ids[i] = other.user_ids[i];
		user_names = new string[size];
		for(int i=0;i<size;i++)
			user_names[i] = other.user_names[i];
	}
	
	bool borrowing_books(const int& usr_id, const string& usr_nm){
		if(!quantity)//no copies available for that book
			return 0;
		user_ids[len] = usr_id;
		user_names[len++] = usr_nm;
		total_borrowed++;
		quantity--;
		return 1;
	}
	
	bool returning_books(const int& usr_id){
		if(!total_borrowed)//no books have been borrowed yet
			return 0;
		int i = 0;
		//get user id index from the list
		for(;i<len;i++){
			if(user_ids[i] == usr_id)
				break;
		}
		if(i == len)//the id is not correct
			return 0;
		for(;i<len-1;i++){//shift left from the id index
			*(user_ids + i) = *(user_ids + i + 1);
			*(user_names + i) = *(user_names + i + 1);
		}
		user_ids[--len] = 0;
		user_names[len] = "";
		total_borrowed--;
		quantity++;
		return 1;
	}
	
	bool match_prefix(const string& pf){
		int size = pf.size();
		for(int i=0;i<size;i++){
			if(pf[i] != name[i])
				return 0;
		}
		return 1;
	}
	
	void print(){
		cout << "id = " << id;
		cout << " name = " << name;
		cout << " total_quantity = " << quantity;
		cout << " total_borrowed = " << total_borrowed << endl;
	}
	
	void print_who_borrowed_by_name(){
		for(int i=0;i<len;i++)
			cout << user_names[i];
	}
};
 
struct Library_system{
	Book books[MAX];
	int books_len = 0;
	User users[MAX];
	int users_len = 0;
	
	Library_system(){}
	
	void add_book(){
		int id, q;
		string nm;
		cout << "Enter book info: id & name & total quantity: ";
		cin >> id >> nm >> q;
		books[books_len++] = Book(id, nm, q);
	}
	
	void search_books_by_prefix(){
		string prefix {""};
		cout << "Enter book name prefix: ";
		cin >> prefix;
		for(int i=0;i<books_len;i++){
			if(books[i].match_prefix(prefix))
				cout << books[i].name << endl; 
		}
	}
	
	static bool compare_by_id(const Book& lhs, const Book& rhs){
		return lhs.id < rhs.id;
	}

	static bool compare_by_name(const Book& lhs, const Book& rhs){
		return lhs.name < rhs.name;
	}
	
	void print_library_by_id(){
		
		sort(books, books + books_len, compare_by_id);
		for(int i=0;i<books_len;i++)
			books[i].print();
	}
	
	void print_library_by_name(){
		sort(books, books + books_len, compare_by_name);
		for(int i=0;i<books_len;i++)
			books[i].print();
	}
	
	void add_user(){
		int id;
		string name;
		cout << "Enter user name & national ID: ";
		cin >> name >> id;
		users[users_len++] = User(id, name);
	}
	
	void print_users(){
		for(int i=0;i<users_len;i++)
			users[i].print();
	}
	
	void user_borrow_book(){
		string book_name, usr_name;
		cout << "Enter user name and book name: ";
		cin >> usr_name >> book_name;
		int book_index = -1;
		int user_index = -1;
		/*get index of book*/
		for(int i=0;i<books_len;i++){
			if(books[i].name == book_name)
				{book_index = i; break;}
		}
		if(book_index == -1){//if book name does not match
			cout << "There is no such book name like that. Please, write correct name later.\n";
			return;
		}
		if(books[book_index].quantity == 0){//if there is no copy available
			cout << "Empty copies, can't be borrowed.\n";
			return;
		}
		/*get user's index*/
		for(int i=0;i<users_len;i++){
			if(users[i].name == usr_name)
				{user_index = i; break;}
		}
		if(user_index == -1){// if user name does not match
			cout << "There is no such user name like that. Please, write correct name later.\n";
			return;
		}
		/*mark that user borrow a book by store book id in user database*/
		int book_id = books[book_index].id;
		if(!users[user_index].borrow_book(book_id))
			cout << "sorry, You've exceeded the maximum number of books you can borrow\n";
		/*mark that user borrow a book by store user id and user name in book database*/
		int user_id = users[user_index].id;
		string user_name = users[user_index].name;
		if(!books[book_index].borrowing_books(user_id, user_name))
			cout << "There is no copies available for this book\n";
	}
	
	void user_return_book(){
 		string book_name, user_name;
		cout << "Enter user name and book name: ";
		cin >> user_name >> book_name;
		int book_index = -1;
		int user_index = -1;
		//get index of book
		for(int i=0;i<books_len;i++){
			if(books[i].name == book_name)
				{book_index = i; break;}
		}
		if(book_index == -1){//if book name does not match
			cout << "There is no such book name like that. Please, write correct name later.\n";
			return;
		}
		if(books[book_index].quantity == 0){//if there is no copy available
			cout << "Empty copies, can't be borrowed.\n";
			return;
		}
		//get user's index
		for(int i=0;i<users_len;i++){
			if(users[i].name == user_name)
				{user_index = i; break;}
		}
		if(user_index == -1){// if user name does not match
			cout << "There is no such user name like that. Please, write correct name later.\n";
			return;
		}
		//mark that user return a book by pop book id from user database
		int book_id = books[book_index].id;
		users[user_index].return_book(book_id);
		//mark that user return a book by pop user id from book database
		int user_id = users[user_index].id;
		books[book_index].returning_books(user_id);
	}
	
	void print_who_borrowed_book_by_name(){
		string name;
		cout << "Enter book name: ";
		cin >> name;
		int book_index = -1;
		/* get book index */
		for(int i=0;i<books_len;i++){
			if(books[i].name == name){
				book_index = i;
				break;
			}
		}
		if(book_index == -1){//if the name does not match
			cout << "There is no book name like \"" << name << "\"\n";
			return;
		}
		int len = books[book_index].len;
		for(int i=0;i<len;i++)
			cout << books[book_index].user_names[i] << endl;
	}
	
	int select_choice(){
		int choice = 0;
			cin >> choice;
		while(1){	
			if(choice > 0 && choice < 11)
				return choice;
			else
				cout << "please, choose from list." << endl;
		}
	}
	
	void system(){
		cout << "\nLibrary Menu;\n";
		cout << "1) add_book\n";
		cout << "2) search_books_by_prefix\n";
		cout << "3) print_who_borrowed_book_by_name\n";
		cout << "4) print_library_by_id\n";
		cout << "5) print_library_by_name\n";
		cout << "6) add_user\n";
		cout << "7) user_borrow_book\n";
		cout << "8) user_return_book\n";
		cout << "9) print_users\n";
		cout << "10) Exit\n";
		while(1){
			cout << "Enter your menu choice [1 - 10]: ";
			int choice = select_choice();
			if(choice == 1)
				add_book();
			else if(choice == 2)
				search_books_by_prefix();
			else if(choice == 3)
				print_who_borrowed_book_by_name();
			else if(choice == 4)
				print_library_by_id();
			else if(choice == 5)
				print_library_by_name();		
			else if(choice == 6)
				add_user();		
			else if(choice == 7)
				user_borrow_book();						
			else if(choice == 8)
				user_return_book();							
			else if(choice == 9)
				print_users	();							
			else
				break;
		}		
	}
};

Library_system library;

int main(){
	library.system();
	
}