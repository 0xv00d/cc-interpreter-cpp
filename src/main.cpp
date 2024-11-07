#include <cstring>
#include <fstream>
#include <iostream>

#include "scanner.hpp"

void run(std::string);

int repl();
std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc == 1) return repl();

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        
        if (!file_contents.empty()) {
            lox::Scanner scanner = lox::Scanner(file_contents);
            auto tokens = scanner.scan_tokens();

            for (lox::Token token: tokens) std::cout << token.to_string() << std::endl;

            return 0;
        }
        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}

void run(std::string source) {

}

int repl() {
    while (true) {
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") break;
    }

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
