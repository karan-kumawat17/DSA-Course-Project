// Creating a Grocery Shop Inventory System:

/* TODO:
1. Update product's info and delete some products.
2. Will try using trie data structure. --- (DONE)
3. First search product by trie   ---- (DONE)
4. Apply filters to products, like price range, etc.  ----- (DONE)
5. I will generate Invoices for customers and owner can retrieve them. (THIS WILL BE DONE BY HASHTABLE).
6. Customer's shopping cart ----- (DONE)

7. Imagine I got stocks by some producer and it isn't selling, so returning back those products.
8. Adding a Expiry Date. (I won't)
*/


#include<bits/stdc++.h>
#include<vector>
#include<iostream>
#include<string>
#include<unordered_map>
#include<deque>
#include<algorithm>
using namespace std;

struct GroceryList {
	string name;
	double price;
	long long int quantity;
	string category;
	GroceryList* prev;
	GroceryList* next;
};

struct ShoppingCartItem {
	string name;
	string category;
	double price;
	long long int quantity;

	ShoppingCartItem(const GroceryList& groceryItem, long long int addedQuantity) :
		name(groceryItem.name),
		category(groceryItem.category),
		price(groceryItem.price),
		quantity(addedQuantity) {}
};


// Searching part:


struct TrieNode {
	// pointer array for child nodes of each node
	TrieNode* childNode[26];
	int wordCount;

	TrieNode() {
		// constructor
		// initialize the wordCnt vaiable with 0
		// initialize every index of childNode array with NULL
		wordCount = 0;
		for (int i = 0; i < 26; i++) {
			childNode[i] = NULL;
		}
	}
};

void insert_key(TrieNode* root, string& key) {
	// Initialize the currentNode pointer with root node
	TrieNode* currentNode = root;

	// Iterate across the length of the string
	for (auto c : key) {

		c = tolower(c);

		if (c < 'a' || c > 'z') {
			cout << "Invalid entry." << endl;
			return;
		}

		// Check if the node exist for the current character in the Trie

		if (currentNode->childNode[c - 'a'] == NULL) {
			// If node for current character does not exisst then make a new node
			TrieNode* newNode = new TrieNode();

			// Keep the reference for the newly created node
			currentNode->childNode[c - 'a'] = newNode;
		}

		// Now, move the current node pointer to the newly created node

		currentNode = currentNode->childNode[c - 'a'];
	}

	// Increment the wordEndCount for the last currentNode pointer this implies that there is a string ending at currentNode.
	currentNode->wordCount++;
}

bool search_key(TrieNode* root, string& key) {
	// Initialize the currentNode pointer with the root node

	TrieNode* currentNode = root;

	// Iterate across the length of the string
	for (auto c : key) {

		c = tolower(c);

		if (c < 'a' || c > 'z') {
			cout << "Invalid search." << endl;
		}

		// Check ig the node exist for the current character in the Trie
		if (currentNode->childNode[c - 'a'] == NULL) {
			// Word doesn't exist
			return false;
		}
		// Move the currentNode pointer to already existing node for current character
		currentNode = currentNode->childNode[c - 'a'];
	}
	return (currentNode->wordCount > 0);
}

bool delete_key(TrieNode* root, string& word)
{
	TrieNode* currentNode = root;
	TrieNode* lastBranchNode = NULL;
	char lastBrachChar = 'a';

	for (auto c : word) {

		c = tolower(c);

		if (currentNode->childNode[c - 'a'] == NULL) {
			return false;
		}
		else {
			int count = 0;
			for (int i = 0; i < 26; i++) {
				if (currentNode->childNode[i] != NULL)
					count++;
			}

			if (count > 1) {
				lastBranchNode = currentNode;
				lastBrachChar = c;
			}
			currentNode = currentNode->childNode[c - 'a'];
		}
	}

	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (currentNode->childNode[i] != NULL)
			count++;
	}

	// Case 1: The deleted word is a prefix of other words 
	// in Trie. 
	if (count > 0) {
		currentNode->wordCount--;
		return true;
	}

	// Case 2: The deleted word shares a common prefix with 
	// other words in Trie. 
	if (lastBranchNode != NULL) {
		lastBranchNode->childNode[lastBrachChar - 'a'] = NULL;
		return true;
	}
	// Case 3: The deleted word does not share any common 
	// prefix with other words in Trie. 
	else {
		root->childNode[word[0] - 'a'] = NULL;
		return true;
	}
}

void deleteTrie(TrieNode* node) {
	if (node == nullptr) {
		return;
	}
	for (int i = 0; i < 26; i++) {
		deleteTrie(node->childNode[i]);
	}
	delete node;
}


// Creating grocery list part:

TrieNode* root = new TrieNode();
TrieNode* root1 = new TrieNode();
//std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>();
vector<string> item;
vector<string> itemCategory;

void create_head_node(GroceryList*& head) {
	head = new GroceryList;

	//cout << "Welcome Sir/Ma'am, we have a variety of things in here. How can I help you today?" << endl;
	cout << "Hello Sir/Ma'am, stocking up the inventory first time:" << endl;
	cout << "In which category do you want to stock this product?" << endl;
	cin >> head->category;
	cout << "Enter product's name: ";
	cin >> head->name;
	cout << "How much quantity do you want to add in the stock?" << endl;
	cin >> head->quantity;
	cout << "What is the price of product?" << endl;
	cin >> head->price;

	head->prev = nullptr;
	head->next = nullptr;

	// Inserting the product name and category into the trie
	insert_key(root, head->name);
	insert_key(root1, head->category);
}

void stock_up_grocery_list(GroceryList* head) {
	GroceryList* ptr, * temp;
	ptr = head;

	/*int count;
	cout << "\nHow many products do you want to stock?" << endl;
	cin >> count;*/
	int count = 1;

	cout << "\nYes Sir, extending product's list..." << endl;
	char choice;

	do {
		temp = new GroceryList;
		temp->next = nullptr;
		temp->prev = nullptr;

		cout << "In which category do you want to stock this product?" << endl;
		cin >> temp->category;
		cout << "Enter product's name: ";
		cin >> temp->name;
		cout << "How much quantity do you want to add in the stock?" << endl;
		cin >> temp->quantity;
		cout << "What is the price of product?" << endl;
		cin >> temp->price;

		insert_key(root, temp->name);
		insert_key(root1, temp->category);

		//GroceryCategory.push_back(temp->category);
		//GroceryName.push_back(temp->name);

		ptr->next = temp;
		temp->prev = ptr;
		ptr = temp;

		count++;

		cout << "\nCurrently there are " << count << " number of products (By this, I don't mean total).\n" << endl;

		cout << "Do you want to continue adding more products (Y/N)?" << endl;
		cin >> choice;

	} while ((choice == 'Y' || choice == 'y'));
}

void insert_key_of_grocery_name(TrieNode* root, TrieNode* root1, vector<string>& item, vector<string>& itemCategory) {
	int n = item.size();
	for (int i = 0; i < n; i++) {
		insert_key(root, item[i]);
		insert_key(root1, itemCategory[i]);
	}
}

void update_inventory(GroceryList* product, long long int quantity) {
	if ((product != nullptr) && product->quantity >= quantity) {
		product->quantity -= quantity;
		cout << "Inventory updated successfully." << endl;
	}
	else {
		cout << "Error: Not enough stock available!!!" << endl;
	}
}

void applyFilters(GroceryList* head) {
	cout << "Do you want to filter by:\n1. Price\n2. Quantity" << endl;
	int choiceOfFilter;
	cin >> choiceOfFilter;
	switch (choiceOfFilter)
	{
	case 1: {
		cout << "What range do you want your product to be in?(Give your choice in low high format)" << endl;
		int lowPrice, highPrice;
		cin >> lowPrice >> highPrice;

		GroceryList* temp1 = head;
		cout << "Category\tName\tPrice\tQuantity" << endl;

		int counter1 = 0;
		while (temp1 != NULL) {
			if ((lowPrice <= temp1->price) && (temp1->price <= highPrice)) {
				cout << temp1->category << "\t" << temp1->name << "\t" << temp1->price << "\t" << temp1->quantity << endl;
				counter1++;
			}
			temp1 = temp1->next;
		}

		if (counter1 == 0) {
			cout << "NO PRODUCT IS WITHIN PRICE RANGE!!" << endl;
			return;
		}
		break;
	}

	case 2: {
		cout << "How many quantities do you want. Based on that, I will refine the inventory." << endl;
		int quantityOfProduct;
		cin >> quantityOfProduct;

		GroceryList* temp2 = head;

		cout << "Category\tName\tPrice\tQuantity" << endl;

		int counter2 = 0;
		while (temp2 != NULL) {
			if (temp2->quantity >= quantityOfProduct) {
				cout << temp2->category << "\t" << temp2->name << "\t" << temp2->price << "\t" << temp2->quantity << endl;
				counter2++;
			}
			temp2 = temp2->next;
		}

		if (counter2 == 0) {
			cout << "SORRY BUT WE ARE LOW ON STOCK!!" << endl;
			return;
		}
		break;
	}
	}
}

vector<GroceryList*> searchGrocery(TrieNode* root, TrieNode* root1, GroceryList* head) {
	vector<GroceryList*> foundProducts;
	string query;
	string choiceOfSearch;
	cout << "Do you want to search grocery by their name or their category?" << endl;
	cin >> choiceOfSearch;

	//  convert choiceOfSearch to lowercase for comparison:
	transform(choiceOfSearch.begin(), choiceOfSearch.end(), choiceOfSearch.begin(), ::tolower);

	if (choiceOfSearch == "name") {
		cout << "Enter the name of product you want to search." << endl;
		cin.ignore();
		cin >> query;

		// convert query to lowercase:
		transform(query.begin(), query.end(), query.begin(), ::tolower);

		GroceryList* temp;
		temp = head;
		//cout << "\nCategory\tName\tQuantity\tPrice\n" << endl;
		if (search_key(root, query)) {
			while (temp != NULL) {

				// Convert the product name to lowercase for comparison
				string productNameLower = temp->name;
				transform(productNameLower.begin(), productNameLower.end(), productNameLower.begin(), ::tolower);

				if (productNameLower == query) {
					//cout << temp->category << "\t" << temp->name << "\t" << temp->quantity << "\t" << temp->price << endl;
					foundProducts.push_back(temp);
				}
				temp = temp->next;
			}
		}
		else {
			cout << "Product with this name not found in inventory." << endl;
		}
	}
	else if (choiceOfSearch == "category") {
		cout << "Enter the category of product you want to search." << endl;
		cin.ignore();
		cin >> query;

		// convert query to lowercase:
		transform(query.begin(), query.end(), query.begin(), ::tolower);

		GroceryList* temp;
		temp = head;
		//cout << "\nCategory\tName\tQuantity\tPrice\n" << endl;
		if (search_key(root1, query)) {
			while (temp != NULL) {

				// Convert the product category to lowercase for comparison
				string productCategoryLower = temp->category;
				transform(productCategoryLower.begin(), productCategoryLower.end(), productCategoryLower.begin(), ::tolower);

				if (productCategoryLower == query) {
					//cout << temp->category << "\t" << temp->name << "\t" << temp->quantity << "\t" << temp->price << endl;
					foundProducts.push_back(temp);
				}
				temp = temp->next;
			}
		}
		else {
			cout << "Product with this category not found in inventory." << endl;
		}
	}

	return foundProducts;
}

/*void insert_key_for_grocery_by_name(TrieNode* root, GroceryList* head) {
	cout << "\nInserting keys for grocery...\n";
	int n = GroceryName.size();

	for (long int i = 0; i < n; i++) {
		insert_key(root, GroceryName[i]);
	}

	string QueryName;
	vector<string> SearchQueryByName;
	cout << "\nInput the name of grocery that you want to search." << endl;
	cin >> QueryName;

	// Convert the query name to lowercase
	transform(QueryName.begin(), QueryName.end(), QueryName.begin(), ::tolower);

	SearchQueryByName.push_back(QueryName);

	int SearchQueries = SearchQueryByName.size();

	for (int i = 0; i < SearchQueries; i++) {
		cout << "Query String: " << SearchQueryByName[i] << "\n";

		// Iterating through the inventory list to find and display items with matching names.
		GroceryList* temp = head;
		bool found = false;

		while (temp != NULL) {
			// Convert the item's name to lowercase for comparison
			string itemNameLower = temp->name;
			transform(itemNameLower.begin(), itemNameLower.end(), itemNameLower.begin(), ::tolower);

			if (temp->name == SearchQueryByName[i]) {
				found = true;
				cout << "Category: " << temp->category << "\tName: " << temp->name << "\tPrice: " << temp->price << "\tQuantity: " << temp->quantity << endl;
			}
			temp = temp->next;
		}

		if (!found) {
			cout << "No items found with specified name." << endl;
		}
	}

}*/

void display_inventory(GroceryList*& head) {
	GroceryList* temp;
	temp = head;
	cout << "\nCategory\tName\tPrice\tQuantity\n";
	while (temp != NULL) {
		item.push_back(temp->name);
		itemCategory.push_back(temp->category);
		cout << temp->category << "\t" << "\t" << temp->name << "\t" << temp->price << "\t" << temp->quantity << endl;
		temp = temp->next;
	}
}

/*void insert_key_of_grocery_name(TrieNode* root, TrieNode* root1, vector<string>& item, vector<string>& itemCategory) {
	int n = item.size();
	for (int i = 0; i < n; i++) {
		insert_key(root, item[i]);
		insert_key(root1, itemCategory[i]);
	}
}*/

/*void display_added_product(ShoppingCartItem& cartItem) {
	cout << "Product added to shopping cart:" << endl;
	cout << "Category\tName\tPrice\tQuantity" << endl;
	cout << cartItem.category << cartItem.name << cartItem.price << cartItem.quantity << endl;
}*/

void display_total_cost(vector<ShoppingCartItem>& shoppingCart) {
	double totalCost = 0;
	cout << "Shopping Cart:" << endl;
	for (const auto& cartItem : shoppingCart) {
		totalCost += cartItem.price * cartItem.quantity;
		cout << "Category: " << cartItem.category << "\tName: " << cartItem.name << "\tPrice: " << cartItem.price << "\tQuantity: " << cartItem.quantity << endl;
	}
	cout << "Total Cost: " << totalCost << endl;
}

/*void addToShoppingCart(GroceryList* head, vector<ShoppingCartItem>& shoppingCart) {
	cout << "\nWelcome Sir/Ma'am. Here is the list of products we currently have, what do you want?" << endl;
	display_inventory(head);
	vector<GroceryList*> foundProduct = searchGrocery(root, root1, head);

	if (!foundProduct.empty() {
		cout << "How much do you want?" << endl;
		long long int quantity;
		cin >> quantity;

		if (foundProduct->quantity >= quantity) {
			ShoppingCartItem cartItem(*foundProduct, quantity);
			cout << "Certainly, adding the product to shopping cart..." << endl;

			shoppingCart.push_back(cartItem);

			//display_total_cost(cartItem);  // shit is wrong herreeeeeeee.
			update_inventory(foundProduct, quantity);
		}
		else {
			cout << "Error: Not enough stock available." << endl;
		}

		// now i want the search grocery function to return a vector so i can easily pick and insert it in it.
	}
	else {
		cout << "Product not found. Cannot add to shopping cart." << endl;
	}
}*/

void addToShoppingCart(GroceryList* head, vector<ShoppingCartItem>& shoppingCart) {
	cout << "\nWelcome Sir/Ma'am. Here is the list of products we currently have, what do you want?" << endl;
	display_inventory(head);

	vector<GroceryList*> foundProducts = searchGrocery(root, root1, head);

	if (!foundProducts.empty()) {
		cout << "Multiple products found with the given search criteria. Please choose one from the list:" << endl;

		for (size_t i = 0; i < foundProducts.size(); ++i) {
			cout << i + 1 << ". Category: " << foundProducts[i]->category << "\tName: " << foundProducts[i]->name
				<< "\tPrice: " << foundProducts[i]->price << "\tQuantity: " << foundProducts[i]->quantity << endl;
		}

		cout << "Enter the number corresponding to the product you want to add to the shopping cart:" << endl;
		int choice;
		cin >> choice;

		if (choice >= 1 && static_cast<size_t>(choice) <= foundProducts.size()) {
			GroceryList* chosenProduct = foundProducts[choice - 1];

			cout << "How much do you want?" << endl;
			long long int quantity;
			cin >> quantity;

			if (chosenProduct->quantity >= quantity) {
				ShoppingCartItem cartItem(*chosenProduct, quantity);
				cout << "Certainly, adding the product to the shopping cart..." << endl;

				shoppingCart.push_back(cartItem);

				update_inventory(chosenProduct, quantity);
			}
			else {
				cout << "Error: Not enough stock available." << endl;
			}
		}
		else {
			cout << "Invalid choice. Please enter a valid number." << endl;
		}
	}
	else {
		cout << "Product not found. Cannot add to shopping cart." << endl;
	}
}

/*void returnProductToInventory(GroceryList* head, vector<ShoppingCartItem> shoppingCart) {
	cout << "Welcome Sir/Ma'am. Here is the list of products in your shopping cart:" << endl;
	display_total_cost(shoppingCart);

	string productName;
	cout << "Enter the name of the product you want." << endl;
	cin >> productName;

	auto foundProduct = find_if(shoppingCart.begin(), shoppingCart.end(), [productName](const ShoppingCartItem& item) {
		string lowerProductName = productName;
		transform(lowerProductName.begin(), lowerProductName.end(), lowerProductName.begin(), ::tolower);

		string lowerItemName = item.name;
		transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

		return lowerItemName == lowerProductName;
		//return item.name == productName;
		});

	if (foundProduct != shoppingCart.end()) {
		// Find the corresponding product in the inventory
		GroceryList* returnedProduct = searchGrocery(root, root1, head);
		if (returnedProduct != nullptr) {
			// Update the inventory and remove the product from the shopping cart
			returnedProduct->quantity += foundProduct->quantity;
			shoppingCart.erase(foundProduct);

			cout << "Product returned successfully.\n";
			return;
		}
		else {
			cout << "Error: Product not found in the inventory.\n";
		}
	}
	else {
		cout << "Error: Product not found in the shopping cart.\n";
	}
}*/

void returnProductToInventory(GroceryList* head, vector<ShoppingCartItem>& shoppingCart) {
	cout << "Welcome Sir/Ma'am. Here is the list of products in your shopping cart:" << endl;
	display_total_cost(shoppingCart);

	string productName;
	cout << "Enter the name of the product you want to return." << endl;
	cin >> productName;

	vector<GroceryList*> foundProducts = searchGrocery(root, root1, head);

	if (!foundProducts.empty()) {
		// Move the products to be returned to a separate vector
		for (GroceryList* inventoryProduct : foundProducts) {
			auto cartProduct = std::find_if(shoppingCart.begin(), shoppingCart.end(), [productName](const ShoppingCartItem& item) {
				string lowerProductName = productName;
				transform(lowerProductName.begin(), lowerProductName.end(), lowerProductName.begin(), ::tolower);

				string lowerItemName = item.name;
				transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);

				return lowerProductName == lowerItemName;
				});

			if (cartProduct != shoppingCart.end()) {
				// Update the inventory:

				long long int returnQuantity;
				cout << "How much do you want to return back?" << endl;
				cin >> returnQuantity;

				if (returnQuantity >= 0 && returnQuantity <= cartProduct->quantity) {
					inventoryProduct->quantity += returnQuantity;
					cartProduct->quantity -= returnQuantity;

					cout << "Product returned successfully.\n" << endl;
				}
				else {
					cout << "Invalid return quantity!!!" << endl;
				}
			}
			else {
				cout << "Product not found in Shopping Cart.\n";
			}
		}
	}
	else {
		cout << "Error: Product not found in the shopping cart.\n";
	}
}

bool OwnerAuthentication() {
	// Defining valid owner credentials:
	string validUserName = "Karan_Kumawat";
	string validPassword = "WALL_E";

	// Asking for credentials:
	cout << "Enter your UserName:" << endl;
	string enteredUserName;
	cin >> enteredUserName;

	cout << "Enter Password:" << endl;
	string enteredPassword;
	cin >> enteredPassword;

	return (enteredUserName == validUserName && enteredPassword == validPassword);
}


int main() {
	cout << "Welcome Sir/Ma'am, we have a variety of things in here. How can I help you today?" << endl;

	GroceryList* head = nullptr;
	vector<ShoppingCartItem> shoppingCart;

	int userChoice;
	int ownerChoice;

	int role;

	if (OwnerAuthentication()) {
		cout << "Owner Authentication Successful. You have access to owner functionalities." << endl;

		while (true) {
			cout << "Are you a customer or owner? Press 1 for owner & 2 for customer." << endl;
			cin >> role;
			if (role == 1) {
				cout << "Welcome Sir/Ma'am. How can I help you today?" << endl;
				cout << "1. Add products to Inventory\n2. Display Inventory\n3. Exit\n";
				cout << "Enter your choice: ";
				cin >> ownerChoice;

				switch (ownerChoice) {
				case 1:
					//addToShoppingCart(head, shoppingCart);
					create_head_node(head);
					stock_up_grocery_list(head);
					break;
				case 2:
					display_inventory(head);
					break;
				case 3:
					//deleteTrie(root);
					break;
				default:
					cout << "Invalid choice. Please try again.\n";
				}
			}
			else if (role == 2) {
				cout << "Welcome Sir/Ma'am. How can I help you today?" << endl;
				cout << "1. Add product to shopping cart\n2. Return product to inventory\n3. Display shopping cart with total cost\n4. Apply Filters\n5. Exit\n";
				cout << "Enter your choice: ";
				cin >> userChoice;

				switch (userChoice) {
				case 1:
					addToShoppingCart(head, shoppingCart);
					break;
				case 2:
					returnProductToInventory(head, shoppingCart);
					break;
				case 3:
					display_total_cost(shoppingCart);
					break;
				case 4:
					applyFilters(head);
					break;
				case 5:
					//deleteTrie(root);
					break;
				default:
					cout << "Invalid choice. Please try again.\n";
				}
			}
			else {
				cout << "Invalid Choice!! Try Again." << endl;
			}
		}
	}
	deleteTrie(root);
	//cout << root << endl;
	return 0;
}
