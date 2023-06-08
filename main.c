#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold user information
typedef struct {
    char username[20];
    char password[20];
    char securityAnswer1[50];
    char securityAnswer2[50];
} User;

// Structure to hold product information
typedef struct {
    int id;
    char productName[50];
    char brandName[20];
    char productCategory[20];
    float buyPrice;
    float sellPrice;
    int stock;
} Product;


typedef struct BSTNode {
    Product product;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

void displayInOrder(BSTNode* root) {
    if (root != NULL) {
        displayInOrder(root->left);
        printf("| %-3d | %-20s | %-15s | %-20s | $%-10.2f | $%-10.2f | %-5d |\n",
               root->product.id, root->product.productName, root->product.brandName,
               root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
               root->product.stock);
        displayInOrder(root->right);
    }
}


void insertProduct(BSTNode** root, Product product) {
    if (*root == NULL) {
        *root = (BSTNode*)malloc(sizeof(BSTNode));
        (*root)->product = product;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else {
        if (product.id == (*root)->product.id) {
            return; // Product with the same ID already exists, no need to insert again
        } else if (product.id < (*root)->product.id) {
            insertProduct(&(*root)->left, product);
        } else {
            insertProduct(&(*root)->right, product);
        }
    }
}


void loadProductsFromFile(BSTNode** root) {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    int maxLines = 100;  // Maximum number of lines to read
    int lineCount = 0;
    Product* products = (Product*)malloc(maxLines * sizeof(Product));  // Dynamic allocation

    while (lineCount < maxLines && !feof(file)) {
        int result = fscanf(file, "%d | %[^|] | %[^|] | %[^|] | %f | %f | %d\n",
                            &products[lineCount].id, products[lineCount].productName,
                            products[lineCount].brandName, products[lineCount].productCategory,
                            &products[lineCount].buyPrice, &products[lineCount].sellPrice,
                            &products[lineCount].stock);

        if (result == 7) {
            lineCount++;
        } else if (result == EOF) {
            break;  // Reached end of file
        } else {
            printf("Error reading line %d\n", lineCount + 1);
            break;
        }
    }

    fclose(file);

    // Insert the products into the BST
    for (int i = 0; i < lineCount; i++) {
        insertProduct(root, products[i]);
    }

    free(products);  // Free the dynamically allocated memory
}


void displayAllProducts(BSTNode* root) {
    if (root == NULL) {
        printf("Warehouse is empty!\n");
    } else {
        printf("\n");
        displayInOrder(root);
    }
}

void deleteBST(BSTNode* root) {
    if (root == NULL) {
        return;
    }

    // Delete left and right subtrees
    deleteBST(root->left);
    deleteBST(root->right);

    // Delete the current node
    free(root);
}

int isIdExists(BSTNode* root, int id) {
    if (root == NULL) {
        return 0; // ID not found
    }

    if (id == root->product.id) {
        return 1; // ID found
    } else if (id < root->product.id) {
        return isIdExists(root->left, id);
    } else {
        return isIdExists(root->right, id);
    }
}


// Function to add a new product
void addNewProduct(Product* product) {
    printf("--- Add New Product ---\n");

    printf("Product Name: ");
    scanf(" %[^\n]s", product->productName);

    printf("Brand Name:\n");
    printf("1. Polytron\n");
    printf("2. LG\n");
    printf("3. Sanken\n");
    printf("4. Miyako\n");
    printf("5. Samsung\n");
    printf("6. Sony\n");
    int brandChoice;
    printf("Enter your choice: ");
    scanf("%d", &brandChoice);

    switch (brandChoice) {
        case 1:
            strcpy(product->brandName, "Polytron");
            break;
        case 2:
            strcpy(product->brandName, "LG");
            break;
        case 3:
            strcpy(product->brandName, "Sanken");
            break;
        case 4:
            strcpy(product->brandName, "Miyako");
            break;
        case 5:
            strcpy(product->brandName, "Samsung");
            break;
        case 6:
            strcpy(product->brandName, "Sony");
            break;
        default:
            printf("Invalid choice. Brand Name set to Unknown.\n");
            strcpy(product->brandName, "Unknown");
            break;
    }

    printf("Product Category:\n");
    printf("1. Kulkas\n");
    printf("2. Mesin Cuci\n");
    printf("3. Kompor Listrik\n");
    printf("4. Magic Com\n");
    printf("5. LED TV\n");
    int categoryChoice;
    printf("Enter your choice: ");
    scanf("%d", &categoryChoice);

    switch (categoryChoice) {
        case 1:
            strcpy(product->productCategory, "Kulkas");
            break;
        case 2:
            strcpy(product->productCategory, "Mesin Cuci");
            break;
        case 3:
            strcpy(product->productCategory, "Kompor Listrik");
            break;
        case 4:
            strcpy(product->productCategory, "Magic Com");
            break;
        case 5:
            strcpy(product->productCategory, "LED TV");
            break;
        default:
            printf("Invalid choice. Product Category set to Unknown.\n");
            strcpy(product->productCategory, "Unknown");
            break;
    }

    printf("Buy Price: ");
    scanf("%f", &product->buyPrice);

    printf("Sell Price: ");
    scanf("%f", &product->sellPrice);

    printf("Stock: ");
    scanf("%d", &product->stock);

    printf("New product added successfully!\n");
}

// Function to generate a unique ID for each product
int generateProductId() {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return -1;
    }

    int maxId = 0;
    int currentId;
    while (fscanf(file, "%d", &currentId) == 1) {
        if (currentId > maxId) {
            maxId = currentId;
        }
        fscanf(file, "%*[^\n]"); // Skip to the end of the line
    }
    fclose(file);

    return maxId + 1;
}

// Function to write product data to a text file
void writeProductDataToFile(Product* product) {
    FILE* file = fopen("products.txt", "a");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    fprintf(file, "%d | %s | %s | %s | %.2f | %.2f | %d\n", product->id, product->productName, product->brandName,
            product->productCategory, product->buyPrice, product->sellPrice, product->stock);
    fclose(file);
}
void searchProduct(BSTNode* root, int id) {
    if (root == NULL) {
        printf("Product not found.\n");
    } else if (id == root->product.id) {
        printf("| %-3d | %-20s | %-15s | %-20s | $%-10.2f | $%-10.2f | %-5d |\n",
               root->product.id, root->product.productName, root->product.brandName,
               root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
               root->product.stock);
               getchar();
    } else if (id < root->product.id) {
        searchProduct(root->left, id);
    } else {
        searchProduct(root->right, id);
    }
}
void writeInOrder(BSTNode* root, FILE* file) {
    if (root != NULL) {
        writeInOrder(root->left, file);
        fprintf(file, "%d | %s | %s | %s | %.2f | %.2f | %d\n",
                root->product.id, root->product.productName, root->product.brandName,
                root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                root->product.stock);
        writeInOrder(root->right, file);
    }
}

void updateProductStock(BSTNode* root, int id, int newStock) {
    if (root == NULL) {
        printf("Product not found.\n");
        return;
    }

    if (id == root->product.id) {
        root->product.stock = newStock;
        printf("Product stock updated successfully!\n");

        // Update the products.txt file
        FILE* file = fopen("products.txt", "r");
        FILE* tempFile = fopen("temp.txt", "w");
        if (file == NULL || tempFile == NULL) {
            printf("Failed to open the file.\n");
            return;
        }

        // Copy the data to a temporary file, updating the stock for the matching product ID
        char line[200];
        while (fgets(line, sizeof(line), file)) {
            int currentId;
            sscanf(line, "%d", &currentId);

            if (currentId == id) {
                fprintf(tempFile, "%d | %s | %s | %s | %.2f | %.2f | %d\n",
                        root->product.id, root->product.productName, root->product.brandName,
                        root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                        root->product.stock);
            } else {
                fputs(line, tempFile);
            }
        }

        fclose(file);
        fclose(tempFile);

        // Rename the temporary file to overwrite the original file
        remove("products.txt");
        rename("temp.txt", "products.txt");
    } else if (id < root->product.id) {
        updateProductStock(root->left, id, newStock);
    } else {
        updateProductStock(root->right, id, newStock);
    }
}





void deleteProductConfirmation(BSTNode** rootPtr, int id) {
    if (*rootPtr == NULL) {
        printf("Product not found.\n");
    } else if (id == (*rootPtr)->product.id) {
        printf("Are you sure you want to delete this product?\n");
        printf("| %-3d | %-20s | %-15s | %-20s | $%-10.2f | $%-10.2f | %-5d |\n",
               (*rootPtr)->product.id, (*rootPtr)->product.productName, (*rootPtr)->product.brandName,
               (*rootPtr)->product.productCategory, (*rootPtr)->product.buyPrice, (*rootPtr)->product.sellPrice,
               (*rootPtr)->product.stock);
        printf("Enter 'Y' to confirm deletion, or any other key to cancel: ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            // Delete the product
            BSTNode* current = *rootPtr;
            BSTNode* parent = NULL;
            BSTNode* successor;
            
            // Case 1: No child or one child
            if (current->left == NULL) {
                *rootPtr = current->right;
                free(current);
            } else if (current->right == NULL) {
                *rootPtr = current->left;
                free(current);
            }
            // Case 2: Two children
            else {
                successor = current->right;
                while (successor->left != NULL) {
                    parent = successor;
                    successor = successor->left;
                }
                current->product = successor->product;
                
                if (parent != NULL)
                    parent->left = successor->right;
                else
                    current->right = successor->right;
                
                free(successor);
            }

            printf("Product deleted successfully!\n");

            // Update the products.txt file
            FILE* file = fopen("products.txt", "r");
            FILE* tempFile = fopen("temp.txt", "w");
            if (file == NULL || tempFile == NULL) {
                printf("Failed to open the file.\n");
                return;
            }

            // Copy the data to a temporary file, excluding the deleted product
            char line[200];
            while (fgets(line, sizeof(line), file)) {
                int currentId;
                sscanf(line, "%d", &currentId);

                if (currentId != id) {
                    fputs(line, tempFile);
                }
            }

            fclose(file);
            fclose(tempFile);

            // Rename the temporary file to overwrite the original file
            remove("products.txt");
            rename("temp.txt", "products.txt");
        } else {
            printf("Deletion canceled.\n");
        }
    } else if (id < (*rootPtr)->product.id) {
        deleteProductConfirmation(&((*rootPtr)->left), id);
    } else {
        deleteProductConfirmation(&((*rootPtr)->right), id);
    }
}





// Function to reset password and update the text file
void resetPassword(User* user) {
    char newPassword[20];
    char confirmPassword[20];

    do {
        printf("Enter new password: ");
        scanf("%s", newPassword);

        printf("Confirm new password: ");
        scanf("%s", confirmPassword);

        if (strcmp(newPassword, confirmPassword) != 0) {
            printf("Passwords do not match. Please try again.\n");
        }
    } while (strcmp(newPassword, confirmPassword) != 0);

    strcpy(user->password, newPassword);
    printf("Password reset successfully!\n");
    getchar();

    // Update the text file with the new password
    FILE* file = fopen("users.txt", "w");
    fprintf(file, "%s %s %s %s", user->username, user->password, user->securityAnswer1, user->securityAnswer2);
    fclose(file);
}
void generateReportByStockDescending(BSTNode* root, FILE* file) {
    if (root != NULL) {
        generateReportByStockDescending(root->right, file);
        fprintf(file, "| %-3d | %-20s | %-15s | %-20s | $%-9.2f | $%-9.2f | %-5d |\n",
                root->product.id, root->product.productName, root->product.brandName,
                root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                root->product.stock);
        generateReportByStockDescending(root->left, file);
    }
}


void generateReportByStockAscending(BSTNode* root, FILE* file) {
    if (root != NULL) {
        generateReportByStockAscending(root->left, file);
        fprintf(file, "| %-3d | %-20s | %-15s | %-20s | $%-9.2f | $%-9.2f | %-5d |\n",
                root->product.id, root->product.productName, root->product.brandName,
                root->product.productCategory, root->product.buyPrice, root->product.sellPrice,
                root->product.stock);
        generateReportByStockAscending(root->right, file);
    }
}
void generateReportByStock(BSTNode* root, int order) {
    FILE* file;
    if (order == 1)
        file = fopen("report_stock_most_to_least.txt", "w");
    else
        file = fopen("report_stock_least_to_most.txt", "w");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    fprintf(file, "| %-3s  | %-20s | %-15s | %-20s | %-10s | %-10s | %-5s |\n",
            "ID", "Product Name", "Brand Name", "Product Category", "Buy Price", "Sell Price", "Stock");

    if (order == 1)
        generateReportByStockDescending(root, file);
    else
        generateReportByStockAscending(root, file);

    fclose(file);
}



// Function to display the main menu
void displayMainMenu() {
    printf("WEREHOUSE GUDANG ELECTRONIK\n");
    printf("\n--- Main Menu ---\n");
    printf("===================\n");
    printf("1. New Product\n");
    printf("2. Show All Product\n");
    printf("3. Update/Delete Product\n");
    printf("4. Generate Reports\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    User user;
    Product product;
    BSTNode* root = NULL;
    loadProductsFromFile(&root);
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }
    fscanf(file, "%s %s %s %s", user.username, user.password, user.securityAnswer1, user.securityAnswer2);
    fclose(file);

    char enteredUsername[20];
    char enteredPassword[20];
    char choice = 'N';
    while (1) {
        printf("Username: ");
        scanf("%s", enteredUsername);
        printf("Password: ");
        scanf("%s", enteredPassword);

        if (strcmp(enteredUsername, user.username) == 0 && strcmp(enteredPassword, user.password) == 0) {
            printf("Login successful!\n");
            getchar();
            system("cls");
            break;
        } else {
            printf("Invalid username or password.\n");

            printf("Forgot password? (Y/N): ");
            scanf(" %c", &choice);

            if (choice == 'Y' || choice == 'y') {
                char securityAnswer1[50];
                char securityAnswer2[50];

                system("cls");
                printf("Please enter additional security below to continue\n");
                printf("When is your birthday? (dd/mm/yyyy): ");
                scanf(" %[^\n]s", securityAnswer1);
                printf("What is your last name?: ");
                scanf(" %[^\n]s", securityAnswer2);

                if (strcmp(securityAnswer1, user.securityAnswer1) == 0 && strcmp(securityAnswer2, user.securityAnswer2) == 0) {
                    resetPassword(&user);
                    system("cls");
                } else {
                    printf("Incorrect security answer. Password reset failed.\n");
                    getchar();
                    system("cls");
                }
            }
            system("cls");
        }
    }

    // To load the text file content into BST
    loadProductsFromFile(&root);
    int input;
    int menuChoice;
    int id;
    do {
        displayMainMenu();
        scanf("%d", &menuChoice);

        switch (menuChoice) {
            case 1:
                system("cls");
                addNewProduct(&product);
                product.id = generateProductId();
                writeProductDataToFile(&product);
                break;
            case 2:
                system("cls");
                printf("|  ID  | Nama Barang          | Nama Brand      | Kategori             | Harga Beli  | Harga Jual  | Stock |\n");
                printf("==============================================================================================================");
                displayAllProducts(root);
                 getchar();
                break;
            case 3:
                system("cls");
                printf("1. Search\t");
                printf("2. Update\t");
                printf("3. Delete\t\n");
                printf("4. Back To Menu\t\n");
                printf("Choice: ");
                scanf("%d", &input);
                    switch (input) {
            case 1:
                system("cls");
                printf("Enter product ID to search: ");
                scanf("%d", &id);
                searchProduct(root, id);
                break;
            case 2:
               int productId;
             int newStock;
            printf("--- Update Product Stock ---\n");
            printf("Enter the product ID: ");
            scanf("%d", &productId);
            printf("Enter the new stock: ");
            scanf("%d", &newStock);
            updateProductStock(root, productId, newStock);
                break;
                
            case 3:
                printf("Enter product ID to delete: ");
                scanf("%d", &id);
                deleteProductConfirmation(&root, id);
                break;
            
            case 4:
                displayMainMenu();
                break;
                    }
             break;

            case 4:
                printf("Generate Reports\n");
                printf("1. Generate report from high to lower ID\n");
                printf("2. generate report from lower to high ID\n");
                printf("Choice: ");
                scanf("%d", &input);
                getchar();
                switch (input) {
                    case 1:
                    system("cls");
                    generateReportByStock(root, 1); 
                    break;
                    case 2:
                    system("cls");
                    generateReportByStock(root, 0); 
                    break;
                }
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        getchar();
        system("cls");


    } while (menuChoice != 5);
        deleteBST(root);

    return 0;
}
