// lab5_B.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <random>

using namespace std;

// Структура для хранения информации о слове: документ и частота
struct WordInfo {
    int doc_id; // номер документа, в котором встречается слово
    int frequency; // сколько раз слово встречается в этом документе
};

// Структура для результата поиска: документ и суммарная частота
struct SearchResult {
    int doc_id;
    int total_freq; // суммарная частота слова (пары слов) в документе
};

// Функция для преобразования строки в нижний регистр
string to_lower(string s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Функция для очистки слова от знаков препинания
string clean_word(string word) {
    string result;
    for (char c : word) {
        if (isalpha(c) || isdigit(c)) {
            result += c;
        }
    }
    return result;
}

// Функция для генерации случайных документов на основе ID студента
vector<string> generate_random_documents(int num_docs, const string& student_id) {
    // Словарь для генерации случайных текстов
    vector<string> dictionary = {
        "hello", "world", "test", "bed", "great", "search", "engine",
        "computer", "science", "programming", "data", "information", "right", "gold", "apple"
    };

    // Используем хэш ID студента как seed для генератора
    hash<string> hasher;
    unsigned seed = hasher(student_id);
    mt19937 gen(seed);
    uniform_int_distribution<> word_count_dis(3, 10); // Длина документа: 3–10 слов
    uniform_int_distribution<> word_index_dis(0, dictionary.size() - 1);

    vector<string> documents(num_docs);
    for (int i = 0; i < num_docs; i++) {
        string doc;
        int word_count = word_count_dis(gen); // Случайное количество слов
        for (int j = 0; j < word_count; j++) {
            doc += dictionary[word_index_dis(gen)] + " ";
        }
        documents[i] = doc;
    }
    return documents;
}

// Сортировка слиянием для результатов поиска по убыванию частоты
void merge(vector<SearchResult>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<SearchResult> left_arr(n1);
    vector<SearchResult> right_arr(n2);

    for (int i = 0; i < n1; i++)
        left_arr[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        right_arr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i].total_freq >= right_arr[j].total_freq) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

void merge_sort(vector<SearchResult>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main() {
    // Хранилище индекса: слово -> список (документ, частота)
    unordered_map<string, vector<WordInfo>> index;
    // Хранилище для пар слов
    unordered_map<string, vector<WordInfo>> pair_index;

    // Ввод ID студента
    string student_id;
    cout << "Enter student ID: ";
    getline(cin, student_id);

    // Ввод количества документов
    int num_docs;
    cout << "Enter number of documents: ";
    cin >> num_docs;
    cin.ignore(); // Очистка буфера

    // Генерация случайных документов на основе ID студента
    vector<string> documents = generate_random_documents(num_docs, student_id);

    // Вывод сгенерированных документов для проверки
    cout << "\nGenerated documents for student " << student_id << ":\n";
    for (int i = 0; i < num_docs; i++) {
        cout << "Document " << i + 1 << ": " << documents[i] << endl;
    }

    // Индексация документов
    for (int i = 0; i < num_docs; i++) {
        stringstream ss(documents[i]);
        string word;
        unordered_map<string, int> word_freq; // Частота слов в текущем документе
        vector<string> words; // Для парного индексирования

        // Разбиваем документ на слова
        while (ss >> word) {
            word = to_lower(clean_word(word));
            if (!word.empty()) {
                word_freq[word]++;
                words.push_back(word);
            }
        }

        // Добавляем слова в индекс
        for (const auto& pair : word_freq) {
            WordInfo info = { i, pair.second };
            index[pair.first].push_back(info);
        }

        // Индексация пар слов
        for (size_t j = 1; j < words.size(); j++) {
            string pair = words[j - 1] + " " + words[j];
            pair_index[pair].push_back({ i, 1 });
        }
    }

    // Обработка запросов
    string command;
    while (true) {
        cout << "\nEnter command (FIND word, FIND_PAIR word1 word2, or EXIT): ";
        getline(cin, command);

        if (command == "EXIT") {
            break;
        }

        stringstream ss(command);
        string action;
        ss >> action;

        if (action == "FIND") {
            string word;
            ss >> word;
            word = to_lower(clean_word(word));

            if (index.find(word) == index.end()) {
                cout << "Word not found in any document." << endl;
                continue;
            }

            // Собираем результаты
            vector<SearchResult> results;
            for (const auto& info : index[word]) {
                results.push_back({ info.doc_id, info.frequency });
            }

            // Сортировка результатов по частоте
            merge_sort(results, 0, results.size() - 1);

            // Вывод результатов
            cout << "Found in documents (sorted by frequency):" << endl;
            for (const auto& res : results) {
                cout << "Document " << res.doc_id + 1 << ": " << res.total_freq << " times" << endl;
            }
        }
        else if (action == "FIND_PAIR") {
            string word1, word2;
            ss >> word1 >> word2;
            word1 = to_lower(clean_word(word1));
            word2 = to_lower(clean_word(word2));
            string pair = word1 + " " + word2;

            if (pair_index.find(pair) == pair_index.end()) {
                cout << "Pair not found in any document." << endl;
                continue;
            }

            // Собираем результаты
            vector<SearchResult> results;
            for (const auto& info : pair_index[pair]) {
                results.push_back({ info.doc_id, info.frequency });
            }

            // Сортировка результатов по частоте
            merge_sort(results, 0, results.size() - 1);

            // Вывод результатов
            cout << "Pair found in documents (sorted by frequency):" << endl;
            for (const auto& res : results) {
                cout << "Document " << res.doc_id + 1 << ": " << res.total_freq << " times" << endl;
            }
        }
        else {
            cout << "Invalid command. Use FIND word, FIND_PAIR word1 word2, or EXIT." << endl;
        }
    }

    system("pause");
    return 0;
}