#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>

class Plant {
private:
    std::string name;
    std::string species;
    std::string last_watered;
    int interval;

    std::string get_today() const {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        char buf[11];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", now);
        return std::string(buf);
    }

public:
    Plant(std::string name, std::string species, std::string last_watered, int interval)
        : name(name), species(species), last_watered(last_watered), interval(interval) {}

    std::string get_name() const { return name; }
    std::string get_species() const { return species; }
    std::string get_last_watered() const { return last_watered; }
    int get_interval() const { return interval; }

    void display() const {
        std::cout << "Name:         " << name << "\n";
        std::cout << "Species:      " << species << "\n";
        std::cout << "Last watered: " << last_watered << "\n";
        std::cout << "Water every:  " << interval << " days\n";
    }

    void mark_watered() {
        last_watered = get_today();
        std::cout << name << " has been watered on " << last_watered << ".\n";
    }
};

class PlantFile {
private:
    std::string filename;

public:
    PlantFile(std::string filename) : filename(filename) {}

    void save(const std::vector<Plant>& plants) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: could not open file for saving.\n";
            return;
        }
        for (const Plant& p : plants) {
            file << p.get_name() << "|"
                 << p.get_species() << "|"
                 << p.get_last_watered() << "|"
                 << p.get_interval() << "\n";
        }
        std::cout << "Plants saved to " << filename << ".\n";
    }

    std::vector<Plant> load() const {
        std::vector<Plant> plants;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: could not open file for loading.\n";
            return plants;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name, species, last_watered, interval_str;
            std::getline(ss, name, '|');
            std::getline(ss, species, '|');
            std::getline(ss, last_watered, '|');
            std::getline(ss, interval_str, '|');
            plants.emplace_back(name, species, last_watered, std::stoi(interval_str));
        }
        std::cout << "Plants loaded from " << filename << ".\n";
        return plants;
    }
};

void print_menu() {
    std::cout << "\n=== Plant Watering Tracker ===\n";
    std::cout << "1. Add new plant\n";
    std::cout << "2. List plants\n";
    std::cout << "3. Save plant list\n";
    std::cout << "4. Load plant list\n";
    std::cout << "5. Water a plant\n";
    std::cout << "6. Quit\n";
    std::cout << "Choose an option: ";
}

void add_plant(std::vector<Plant>& plants) {
    std::string name, species;
    int interval;

    std::cout << "Plant name: ";
    std::getline(std::cin, name);
    std::cout << "Species: ";
    std::getline(std::cin, species);
    std::cout << "Watering interval (days): ";
    std::cin >> interval;
    std::cin.ignore();

    plants.emplace_back(name, species, "never", interval);
    std::cout << name << " added!\n";
}

void list_plants(const std::vector<Plant>& plants) {
    if (plants.empty()) {
        std::cout << "No plants in your list yet.\n";
        return;
    }
    for (int i = 0; i < plants.size(); i++) {
        std::cout << "\n[" << i + 1 << "]\n";
        plants[i].display();
    }
}

void water_plant(std::vector<Plant>& plants) {
    if (plants.empty()) {
        std::cout << "No plants to water.\n";
        return;
    }
    list_plants(plants);
    std::cout << "\nEnter plant number to water: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice < 1 || choice > plants.size()) {
        std::cout << "Invalid selection.\n";
        return;
    }
    plants[choice - 1].mark_watered();
}

int main() {
    std::vector<Plant> plants;
    PlantFile pf("plants.txt");
    int choice;

    while (true) {
        print_menu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: add_plant(plants);       break;
            case 2: list_plants(plants);     break;
            case 3: pf.save(plants);         break;
            case 4: plants = pf.load();      break;
            case 5: water_plant(plants);     break;
            case 6:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid option, try again.\n";
        }
    }
}