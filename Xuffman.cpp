// Xuffman.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>

using namespace std;
//узел дерева
class Node
{
public:
    int count;//размер
    char symbol;//буква
    Node* left;//левый потомок
    Node* right;//правый потомок

    Node() { }

    Node(char __symbol, int __count)
    {
        symbol = __symbol;
        count = __count;
    }

    Node(Node* l, Node* r)
    {
        symbol = 0;
        left = l;
        right = r;
        count = l->count + r->count;
    }

    static void Print(Node* root, int depth = 0)
    {
        if (!root) return;

        if (root->symbol)
        {
            for (int i = 0; i < depth; i++)
                cout << ".";
            cout << root->symbol << endl;
        }
        else depth++;
        Print(root->left, depth);
        Print(root->right, depth);
    }
};
//строим таблицу
void BuildTable(Node* root, vector<bool>& code, map<char, vector<bool>>& table)
{
    if (root->left)
    {
        code.push_back(0);
        BuildTable(root->left, code, table);
    }

    if (root->right)
    {
        code.push_back(1);
        BuildTable(root->right, code, table);
    }

    if (root->symbol)
        table[root->symbol] = code;
    if (code.size())
        code.pop_back();
}

bool SortNode(const Node* a, const Node* b)
{
    return a->count < b->count;
}
//Декодируем
string Decode(string& str, map<vector<bool>, char>& table)
{
    string out = "";
    vector<bool> code;
    for (int i = 0; i < str.length(); i++)
    {
        code.push_back(str[i] == '0' ? false : true);
        if (table[code])
        {
            out += table[code];
            code.clear();
        }
    }
    return out;
}
// Реализация алгоритма кодирования Хаффмана на C++
int main()
{
    ////// считаем частоты символов	
    ifstream f("1.txt", ios::out | ios::binary);

    map<char, int> m;

    while (!f.eof())
    {
        char c = f.get();
        m[c]++;
    }
    int a = 0, c;
    cout << "Enter the text:" << endl;
    string raw;
    getline(cin, raw);
    map<char, int> symbols;

    for (int i = 0; i < raw.length(); i++)
        symbols[raw[i]]++;

    list<Node*> trees;
    map<char, int>::iterator itr;
    for (itr = symbols.begin(); itr != symbols.end(); itr++)
    {
        Node* p = new Node(itr->first, itr->second);
        trees.push_back(p);
    }
    if (trees.size() == 0) {
        cout << "String is empty" << endl;
        system("pause");
        return 0;
    }
    else
    {
        if (trees.size() == 1)
        {
            Node* root = trees.front();
            root->Print(root);
            cout << " - " << a << endl;
            cout << a << endl;
            system("pause");
        }
        else
        {
            while (trees.size() != 1)

            {
                trees.sort(SortNode);

                Node* l = trees.front();
                trees.pop_front();
                Node* r = trees.front();
                trees.pop_front();

                Node* parent = new Node(l, r);//создаем новый родительский узел с двумя дочерними
                trees.push_back(parent);
            }

            Node* root = trees.front();
            root->Print(root);

            vector<bool> code;
            map<char, vector<bool> > table;
            BuildTable(root, code, table);
            ////// Выводим коды в файл output.txt

            f.clear(); f.seekg(0); // перемещаем указатель снова в начало файла

            ofstream g("output.txt", ios::out | ios::binary);

            int count = 0; char buf = 0;
            while (!f.eof())
            {
                char c = f.get();
                vector<bool> x = table[c];
                for (int n = 0; n < x.size(); n++)
                {
                    buf = buf | x[n] << (7 - count);
                    count++;
                    if (count == 8) { count = 0;   g << buf; buf = 0; }
                }
            }
            f.close();
            g.close();

            ///// считывание из файла output.txt и преобразование обратно

            ifstream F("output.txt", ios::in | ios::binary);

            setlocale(LC_ALL, "Russian"); // чтоб русские символы отображались в командной строке

            Node* p = root;
            count = 0; char byte;
            byte = F.get();
            while (!F.eof())
            {
                bool b = byte & (1 << (7 - count));
                if (b) p = p->right; else p = p->left;
                if (p->left == NULL && p->right == NULL) { cout << p->symbol; p = root; }
                count++;
                if (count == 8) { count = 0; byte = F.get(); }
            }

            F.close();

            for (itr = symbols.begin(); itr != symbols.end(); itr++)
            {
                cout << itr->first << " - ";
                for (int j = 0; j < table[itr->first].size(); j++)
                    cout << table[itr->first][j];
                cout << endl;
            }
            string out = "";

            for (int i = 0; i < raw.length(); i++)
                for (int j = 0; j < table[raw[i]].size(); j++)
                {
                    out += table[raw[i]][j] + '0';
                    cout << table[raw[i]][j];
                }
            cout << endl;
            cout << out.c_str() << endl;



            map<vector<bool>, char> ftable;
            for (auto i = table.begin(); i != table.end(); i++)
                ftable[i->second] = i->first;
            cout << Decode(out, ftable).c_str() << endl;


            while (true);

            system("pause");
        }
    }
