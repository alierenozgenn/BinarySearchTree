#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>

using namespace std;

// İkili Arama Ağacı (BST) için düğüm sınıfı
class Node {
public:
    int data;
    Node* left;
    Node* right;
    
    // Kurucu fonksiyon
    Node(int value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

// İkili Arama Ağacı (BST) sınıfı
class BinarySearchTree {
private:
    Node* root;
    
    // Özyinelemeli ekleme fonksiyonu
    Node* insertRecursive(Node* current, int value) {
        // Eğer ağaç boşsa yeni bir düğüm oluştur
        if (current == nullptr) {
            return new Node(value);
        }
        
        // Aynı değere izin verilmiyor (gerekirse değiştirilebilir)
        if (value == current->data) {
            cout << "Uyarı: " << value << " zaten ağaçta bulunuyor. Ekleme yapılmadı." << endl;
            return current;
        }
        
        // BST kurallarına göre uygun alt ağaca ekle
        if (value < current->data) {
            current->left = insertRecursive(current->left, value);
        } else {
            current->right = insertRecursive(current->right, value);
        }
        
        return current;
    }
    
    // Özyinelemeli silme fonksiyonu
    Node* deleteRecursive(Node* current, int value) {
        // Eğer ağaç boşsa
        if (current == nullptr) {
            return nullptr;
        }
        
        // Silinecek değeri ara
        if (value < current->data) {
            current->left = deleteRecursive(current->left, value);
        } else if (value > current->data) {
            current->right = deleteRecursive(current->right, value);
        } else {
            // Silinecek düğümü bulduk
            
            // Durum 1: Yaprak düğüm (hiç çocuğu yok)
            if (current->left == nullptr && current->right == nullptr) {
                delete current;
                return nullptr;
            }
            
            // Durum 2: Tek çocuklu düğüm
            else if (current->left == nullptr) {
                Node* temp = current->right;
                delete current;
                return temp;
            }
            else if (current->right == nullptr) {
                Node* temp = current->left;
                delete current;
                return temp;
            }
            
            // Durum 3: İki çocuklu düğüm
            // Sağ alt ağaçtaki en küçük değeri bul
            Node* temp = findMin(current->right);
            
            // Bu değeri silinecek düğümün değeriyle değiştir
            current->data = temp->data;
            
            // Sağ alt ağaçtan bu değeri sil
            current->right = deleteRecursive(current->right, temp->data);
        }
        
        return current;
    }
    
    // Bir düğümün alt ağacındaki en küçük değeri bulan yardımcı fonksiyon
    Node* findMin(Node* node) {
        Node* current = node;
        
        // En sola git
        while (current && current->left != nullptr) {
            current = current->left;
        }
        
        return current;
    }
    
    // Ağacı görsel olarak yazdırmak için özyinelemeli yardımcı fonksiyon
    void displayTreeRecursive(Node* root, int space, int height) {
        // Taban durum
        if (root == nullptr) {
            return;
        }
        
        // Boşluk artır
        space += height;
        
        // Önce sağ alt ağacı yazdır
        displayTreeRecursive(root->right, space, height);
        
        // Mevcut düğümü yazdır
        cout << endl;
        for (int i = height; i < space; i++) {
            cout << " ";
        }
        cout << root->data << "\\";
        
        // Sol alt ağacı yazdır
        displayTreeRecursive(root->left, space, height);
    }
    
    // Maksimum elemanı ve seviyesini bulan yardımcı fonksiyon
    void findMaxAndLevel(Node* node, int& maxValue, int& maxLevel, int currentLevel) {
        if (node == nullptr) {
            return;
        }
        
        if (node->data > maxValue) {
            maxValue = node->data;
            maxLevel = currentLevel;
        }
        
        findMaxAndLevel(node->left, maxValue, maxLevel, currentLevel + 1);
        findMaxAndLevel(node->right, maxValue, maxLevel, currentLevel + 1);
    }
    
public:
    // Kurucu fonksiyon
    BinarySearchTree() {
        root = nullptr;
    }
    
    // Yıkıcı fonksiyon
    ~BinarySearchTree() {
        // Tüm düğümleri temizle
        deleteTree(root);
    }
    
    // Tüm ağacı temizleyen yardımcı fonksiyon
    void deleteTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        
        // Önce alt ağaçları temizle
        deleteTree(node->left);
        deleteTree(node->right);
        
        // Sonra mevcut düğümü sil
        delete node;
    }
    
    // Ağaca yeni bir eleman ekler
    void insert(int value) {
        root = insertRecursive(root, value);
        cout << value << " ağaca eklendi." << endl;
    }
    
    // Ağaçtan bir eleman siler
    void remove(int value) {
        Node* oldRoot = root;
        root = deleteRecursive(root, value);
        
        if (oldRoot == root) {
            cout << "Hata: " << value << " ağaçta bulunamadı." << endl;
        } else {
            cout << value << " ağaçtan silindi." << endl;
        }
    }
    
    // Ağacı görsel olarak ekrana yazdırır
    void display() {
        if (root == nullptr) {
            cout << "Ağaç boş!" << endl;
            return;
        }
        
        cout << "Ağaç Görünümü:" << endl;
        displayTreeRecursive(root, 0, 10);
        cout << endl;
    }
    
    // Seviye sırası taraması (BFS) yapar ve sonucu dizi olarak döndürür
    vector<int> levelOrderTraversal() {
        vector<int> result;
        if (root == nullptr) {
            return result;
        }
        
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            
            result.push_back(current->data);
            
            if (current->left != nullptr) {
                q.push(current->left);
            }
            
            if (current->right != nullptr) {
                q.push(current->right);
            }
        }
        
        return result;
    }
    
    // Maksimum elemanı ve seviyesini bulur
    pair<int, int> findMaxElementAndLevel() {
        if (root == nullptr) {
            return make_pair(-1, -1); // Ağaç boşsa
        }
        
        int maxValue = root->data;
        int maxLevel = 0;
        
        findMaxAndLevel(root, maxValue, maxLevel, 0);
        
        return make_pair(maxValue, maxLevel);
    }
};

// Ana fonksiyon
int main() {
    BinarySearchTree bst;
    int choice, value;
    
    cout << "İkili Arama Ağacı (BST) Programı" << endl;
    
    while (true) {
        cout << "\nİşlem Seçin:" << endl;
        cout << "1. Ekleme (Insertion)" << endl;
        cout << "2. Silme (Deletion)" << endl;
        cout << "3. Görüntüleme (Display)" << endl;
        cout << "4. Seviye Sırası Taraması (Level Order Traversal)" << endl;
        cout << "5. Maksimum Eleman ve Seviyesi" << endl;
        cout << "6. Çıkış (Exit)" << endl;
        cout << "Seçiminiz: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Eklenecek değer: ";
                cin >> value;
                bst.insert(value);
                break;
                
            case 2:
                cout << "Silinecek değer: ";
                cin >> value;
                bst.remove(value);
                break;
                
            case 3:
                bst.display();
                break;
                
            case 4:
                {
                    vector<int> levelOrder = bst.levelOrderTraversal();
                    cout << "Seviye Sırası Taraması: [";
                    for (size_t i = 0; i < levelOrder.size(); i++) {
                        cout << levelOrder[i];
                        if (i < levelOrder.size() - 1) {
                            cout << ", ";
                        }
                    }
                    cout << "]" << endl;
                }
                break;
                
            case 5:
                {
                    pair<int, int> maxInfo = bst.findMaxElementAndLevel();
                    if (maxInfo.first != -1) {
                        cout << "Maksimum Eleman: " << maxInfo.first << ", Seviye: " << maxInfo.second << endl;
                    } else {
                        cout << "Ağaç boş!" << endl;
                    }
                }
                break;
                
            case 6:
                cout << "Program sonlandırılıyor..." << endl;
                return 0;
                
            default:
                cout << "Geçersiz seçim! Lütfen tekrar deneyin." << endl;
        }
    }
    
    return 0;
}