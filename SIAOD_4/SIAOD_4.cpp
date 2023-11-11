// SIAOD_4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;

    Node(char val) {
        this->left = nullptr;
        this->right = nullptr;
        this->data = val;
    }

    bool isLeaf() {
        if (this == nullptr) return false;
        if (this->left == nullptr && this->right == nullptr) return true;
        return false;
    }
    
};


Node* createExpressionTree(string& expression, int leftIndex, int rightIndex) {
    //if (expression.empty()) return nullptr;
    if (rightIndex == leftIndex) {
        Node* newNode = new Node(expression[leftIndex]);
        return newNode;
    }
    bool operatorFound = false;
    for (int i = rightIndex; i >= leftIndex; i--) {
        if (expression[i] == '-' || expression[i] == '+') {
            operatorFound = true;
            Node* newNode = new Node(expression[i]);
            newNode->left = createExpressionTree(expression, leftIndex, i - 1);
            newNode->right = createExpressionTree(expression, i + 1, rightIndex);
            return newNode;
            break;
        }
    }
    if (!operatorFound) {
        for (int i = rightIndex; i >= leftIndex; i--) {
            if (expression[i] == '/' || expression[i] == '*') {
                operatorFound = true;
                Node* newNode = new Node(expression[i]);
                newNode->left = createExpressionTree(expression, leftIndex, i - 1);
                newNode->right = createExpressionTree(expression, i + 1, rightIndex);
                return newNode;
                break;
            }
        }
    }
    if (!operatorFound) return nullptr;

}


void printExpressionTree(Node* expressionTree, int level = 0) {
    
    if (expressionTree == nullptr) {
        //cout << "No such expression tree";
        return;
    }
    if (expressionTree->right != nullptr) {
        printExpressionTree(expressionTree->right, level + 1);
        cout << "\n";
    }
    for (int i = 0; i < level; i++) cout << "   ";
    cout << expressionTree->data;
    if (expressionTree->left != nullptr) {
        cout << "\n";
        printExpressionTree(expressionTree->left, level + 1);
    }
   
    
}

//задания варианта
void simplifyTheTreeOnce(Node* tree, bool* simplified) {
    if (tree == nullptr) return;
    bool treeIsModified = false; //если дерево будет изменено, функцию нужно запустить ещё раз
    
    simplifyTheTreeOnce(tree->left, simplified);
    simplifyTheTreeOnce(tree->right, simplified);

    if (tree->left != nullptr && tree->right != nullptr) {
        if (tree->data == '+') {
            if (tree->left->data == '0') {
                delete tree->left;
                tree->data = tree->right->data;
                tree->left = tree->right->left;
                tree->right = tree->right->right;
                *simplified = true;
            }
            else if (tree->right->data == '0') {
                delete tree->right;
                tree->data = tree->left->data;
                tree->right = tree->left->right;
                tree->left = tree->left->left;
                *simplified = true;
            }
        }
        else if (tree->data == '-' && tree->right->data == '0') {
            delete tree->right;
            tree->data = tree->left->data;
            tree->right = tree->left->right;
            tree->left = tree->left->left;
            *simplified = true;
        }
        else if (tree->data == '*') {
            if (tree->left->data == '1') {
                delete tree->left;
                tree->data = tree->right->data;
                tree->left = tree->right->left;
                tree->right = tree->right->right;
                *simplified = true;
            }
            else if (tree->right->data == '1') {
                delete tree->right;
                tree->data = tree->left->data;
                tree->right = tree->left->right;
                tree->left = tree->left->left;
                *simplified = true;
            }
            else if (tree->right->data == '0' || tree->left->data == '0') {
                tree->data = '0';
                delete tree->left;
                delete tree->right;
                tree->left = nullptr;
                tree->right = nullptr;
                *simplified = true;
            }
        }
        
    }
    
    
    

}

void simplifyTheTreeCompletely(Node* tree) {
    bool simplified;
    do {
        simplified = false;
        simplifyTheTreeOnce(tree, &simplified);
    } while (simplified);
}

void bfsTraversalWithIndent(Node* root) {
    if (root == nullptr) {
        return;
    }

    // Создаем очередь для хранения узлов дерева и их уровней
    std::queue<std::pair<Node*, int>> q;

    // Добавляем корневой узел и его уровень в очередь
    q.push({ root, 0 });

    int currentLevel = 0;

    while (!q.empty()) {
        // Извлекаем узел и его уровень из очереди
        Node* current = q.front().first;
        int level = q.front().second;
        q.pop();

        // Если уровень изменился, выводим перевод строки
        if (level != currentLevel) {
            std::cout << std::endl;
            currentLevel = level;
        }

        // Выводим значение узла с отступами
        std::cout << std::setw(4) << current->data;

        // Добавляем левый и правый дочерние узлы и их уровни в очередь
        if (current->left != nullptr) {
            q.push({ current->left, level + 1 });
        }
        if (current->right != nullptr) {
            q.push({ current->right, level + 1 });
        }
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    int command = 1;
    string exp;
    string& expression = exp;
    Node* expressionTree = nullptr;
    while (command != -1) {
        cout << "Введите 1, чтобы создать дерево выражений в соответствии с вводимым выражением\n"
            << "Введите 2, чтобы отобразить дерево повернутым справа налево\n"
            << "Введите 3, чтобы упростить дерево\n"
            << "Введите 4, чтобы вывести дерево, используя алгоритм обхода в ширину\n"
            << "Введите -1, чтобы завершить работу программы\n";
        cin >> command;
        switch (command) {
        case 1:
            cout << "Введите исходное выражение\n";
            cin >> expression;
            expressionTree = createExpressionTree(expression, 0, expression.size() - 1);
            cout << "Дерево построено\n";
            break;
        case 2:
            printExpressionTree(expressionTree);
            cout << "\n";
            break;
        case 3:
            simplifyTheTreeCompletely(expressionTree);
            cout << "Ваше дерево упрощено\n";
            break;
        case 4:
            bfsTraversalWithIndent(expressionTree);
            cout << "\n";
            break;
        case -1:
            break;
        default:
            cout << "Команда введена неверно\n";
            break;
        }

    }



    
    //cout << "Enter expression: ";
    ////cin >> exp;
    //
    //Node* expressionTree = createExpressionTree(expression, 0, expression.size() - 1);
    //cout << "Your expression tree:\n";
    //printExpressionTree(expressionTree);
    //cout << "\nSimplified expression tree:\n";
    //bfsTraversalWithIndent(expressionTree);
    ////printTree(expressionTree);
    //return 0;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
