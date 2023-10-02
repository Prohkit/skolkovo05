#include <fstream>
#include <set>
#include <queue>
#include "iostream"

bool isClientName(std::string const &str) {
    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz0987654321_") ==
           std::string::npos;
}

bool isCorrectTime(std::string const &str) {
    if (str.length() != 5) {
        return false;
    }
    for (int i = 0; i < str.length(); i++) {
        if (i == 2) {
            if (str[i] != ':') {
                return false;
            }
        } else if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    std::string token;
    int table_count;
    std::ifstream file(argv[1]);
    getline(file, token);

    try {
        table_count = std::stoi(token);
    } catch (std::exception) {
        std::cout << 34 << std::endl;
        return 0;
    }

    std::getline(file, token);
    int delim_position = 5;
    std::string begin_time = token.substr(0, delim_position);
    if (!isCorrectTime(begin_time)) {
        std::cout << 42 << std::endl;
        return 0;
    }

    std::string end_time = token.substr(delim_position + 1, token.length());
    if (!isCorrectTime(end_time)) {
        std::cout << 48 << std::endl;
        return 0;
    }

    getline(file, token);
    int cost_per_hour;

    try {
        cost_per_hour = std::stoi(token);
    } catch (std::exception) {
        std::cout << 58 << std::endl;
        return 0;
    }

    std::multiset<std::string> clients_in_club;
    std::string tables[table_count];
    std::queue<std::string> queue;
    int earnings[table_count];
    std::string times[table_count];
    std::string start_times[table_count];

    for (int i = 0; i < table_count; ++i) {
        tables[i] = "empty";
        times[i] = "00:00";
        earnings[i] = 0;
        start_times[i] = "00:00";
    }

    int begin_hours = std::stoi(begin_time.substr(0, 2));
    int begin_minutes = std::stoi(begin_time.substr(3, begin_time.length()));
    int end_hours = std::stoi(end_time.substr(0, 2));
    int end_minutes = std::stoi(end_time.substr(3, end_time.length()));
    std::cout << begin_time << std::endl;

    while (getline(file, token)) {
        delim_position = 5;
        std::string current_time = token.substr(0, delim_position);

        if (!isCorrectTime(current_time)) {
            std::cout << 86 << std::endl;
            return 0;
        }
        int event_id;

        try { event_id = std::stoi(token.substr(delim_position, delim_position)); }
        catch (std::exception) {
            std::cout << 94 << std::endl;
            return 0;
        }

        std::string client_name;
        int table_number = 0;

        bool client_name_check_flag = false;
        if (event_id == 2) {
            int start_position = delim_position + 3;
            for (int i = start_position; i < token.length(); ++i) {
                if (token[i] == ' ') {
                    delim_position = i;
                    break;
                }
            }
            client_name = token.substr(start_position, delim_position - start_position);
            client_name_check_flag = true;

            try { table_number = std::stoi(token.substr(delim_position, 2)); }
            catch (std::exception) {
                std::cout << 115 << std::endl;
                return 0;
            }
        } else {
            client_name = token.substr(delim_position + 3, token.length());
        }

        if (!isClientName(client_name)) {
            if (client_name_check_flag) {
                std::cout << 112 << std::endl;
                return 0;
            } else {
                std::cout << 121 << std::endl;
                return 0;
            }
        }

        int current_hours = std::stoi(current_time.substr(0, 2));
        int current_minutes = std::stoi(current_time.substr(3, current_time.length()));

        if (event_id == 1) {
            bool open_flag = (begin_hours > current_hours || end_hours < current_hours) ||
                             ((begin_hours == current_hours && begin_minutes > current_minutes) ||
                              (end_hours == current_hours && end_minutes < current_minutes));

            if (open_flag) {
                std::cout << token << std::endl;
                std::cout << current_time + " 13 NotOpenYet" << std::endl;
                continue;
            }
            if (clients_in_club.count(client_name)) {
                std::cout << token << std::endl;
                std::cout << current_time + " 13 YouShallNotPass" << std::endl;
                continue;
            } else {
                clients_in_club.insert(client_name);
                std::cout << token << std::endl;
                continue;
            }
        }

        if (event_id == 2) {
            if (!clients_in_club.count(client_name)) {
                std::cout << token << std::endl;
                std::cout << current_time + " 13 ClientUnknown" << std::endl;
            }
            if (tables[table_number - 1] == "empty") {
                for (int i = 0; i < table_count; i++) {
                    if (tables[i] == client_name) {
                        int start_hours = std::stoi(start_times[i].substr(0, 2));
                        int start_minutes = std::stoi(start_times[i].substr(3, start_times[i].length()));
                        int spent_time;

                        spent_time = (current_hours - start_hours - 1) * 60 + (60 - start_minutes + current_minutes);

                        int spent_hours = (spent_time / 60);
                        int spent_minutes = (spent_time % 60);

                        int sum_spent_time = spent_time + std::stoi(times[i].substr(0, 2)) * 60
                                             + std::stoi(times[i].substr(3, start_times[i].length()));

                        int sum_spent_hours = sum_spent_time / 60;
                        int sum_spent_minutes = sum_spent_time % 60;

                        if (sum_spent_hours < 10) {
                            if (sum_spent_minutes < 10) {
                                times[i] = "0" + std::to_string(sum_spent_hours) + ":0" + std::to_string(sum_spent_minutes);
                            } else {
                                times[i] = "0" + std::to_string(sum_spent_hours) + ":" + std::to_string(sum_spent_minutes);
                            }
                        } else {
                            if (sum_spent_minutes < 10) {
                                times[i] = std::to_string(sum_spent_hours) + ":0" + std::to_string(sum_spent_minutes);
                            } else {
                                times[i] = std::to_string(sum_spent_hours) + ":" + std::to_string(sum_spent_minutes);
                            }
                        }

                        int hours_for_payment = spent_hours;
                        if (spent_minutes != 0) {
                            hours_for_payment = hours_for_payment + 1;
                        }
                        earnings[i] = earnings[i] + (hours_for_payment * cost_per_hour);

                        tables[i] = "empty";
                        start_times[i] = "00:00";
                        break;
                    }
                }

                tables[table_number - 1] = client_name;
                start_times[table_number - 1] = current_time;
                std::cout << token << std::endl;
            } else {
                std::cout << token << std::endl;
                std::cout << current_time + " 13 PlaceIsBusy" << std::endl;
            }
        }

        if (event_id == 3) {
            bool tables_empty_flag = false;
            for (int i = 0; i < table_count; i++) {
                if (tables[i] == "empty") {
                    std::cout << token << std::endl;
                    std::cout << current_time + " 13 ICanWaitNoLonger!" << std::endl;
                    tables_empty_flag = true;
                    break;
                }
            }
            if (tables_empty_flag) continue;

            if (queue.size() == table_count) {
                //ID 11
                clients_in_club.erase(client_name);
                std::cout << token << std::endl;
            } else {
                queue.push(client_name);
                std::cout << token << std::endl;
            }
        }

        if (event_id == 4) {
            std::cout << token << std::endl;
            if (clients_in_club.count(client_name) == 0) {
                std::cout << current_time + " 13 ClientUnknown!" << std::endl;
                break;
            }

            int index;

            for (int i = 0; i < table_count; i++) {
                if (tables[i] == client_name) {
                    int start_hours = std::stoi(start_times[i].substr(0, 2));
                    int start_minutes = std::stoi(start_times[i].substr(3, start_times[i].length()));
                    int spent_time;

                    spent_time = (current_hours - start_hours - 1) * 60 + (60 - start_minutes + current_minutes);

                    int spent_hours = (spent_time / 60);
                    int spent_minutes = (spent_time % 60);

                    int sum_spent_time = spent_time + std::stoi(times[i].substr(0, 2)) * 60
                                         + std::stoi(times[i].substr(3, start_times[i].length()));

                    int sum_spent_hours = sum_spent_time / 60;
                    int sum_spent_minutes = sum_spent_time % 60;

                    if (sum_spent_hours < 10) {
                        if (sum_spent_minutes < 10) {
                            times[i] = "0" + std::to_string(sum_spent_hours) + ":0" + std::to_string(sum_spent_minutes);
                        } else {
                            times[i] = "0" + std::to_string(sum_spent_hours) + ":" + std::to_string(sum_spent_minutes);
                        }
                    } else {
                        if (sum_spent_minutes < 10) {
                            times[i] = std::to_string(sum_spent_hours) + ":0" + std::to_string(sum_spent_minutes);
                        } else {
                            times[i] = std::to_string(sum_spent_hours) + ":" + std::to_string(sum_spent_minutes);
                        }
                    }

                    int hours_for_payment = spent_hours;
                    if (spent_minutes != 0) {
                        hours_for_payment = hours_for_payment + 1;
                    }
                    earnings[i] = earnings[i] + (hours_for_payment * cost_per_hour);
                    index = i;
                    break;
                }
            }
            if (!queue.empty()) {
                //ID 12
                tables[index] = queue.front();
                start_times[index] = current_time;
                std::cout << current_time + " " << 12 << " " + queue.front() + " " << index + 1 << std::endl;
                queue.pop();
                clients_in_club.erase(client_name);
            } else {
                tables[index] = "empty";
                start_times[index] = "00:00";
                clients_in_club.erase(client_name);
            }
        }
    }
    // in multiset the items sorted automatically
    std::multiset<std::string>::iterator it = clients_in_club.begin();
    while (it != clients_in_club.end()) {
        std::multiset<std::string>::iterator current = it++;
        std::string client_name = *current;
        for (int i = 0; i < table_count; i++) {
            if (tables[i] == client_name) {
                int start_hours = std::stoi(start_times[i].substr(0, 2));
                int start_minutes = std::stoi(start_times[i].substr(3, start_times[i].length()));
                int spent_time = 0;

                spent_time = (end_hours - start_hours - 1) * 60 + (60 - start_minutes + end_minutes);

                int spent_hours = (spent_time / 60);
                int spent_minutes = (spent_time % 60);

                int sum_spent_time = spent_time + std::stoi(times[i].substr(0, 2)) * 60
                                     + std::stoi(times[i].substr(3, start_times[i].length()));

                int sum_spent_hours = sum_spent_time / 60;
                int sum_spent_minutes = sum_spent_time % 60;

                if (sum_spent_hours < 10) {
                    if (sum_spent_minutes < 10) {
                        times[i] = "0" + std::to_string(sum_spent_hours) + ":0" + std::to_string(sum_spent_minutes);
                    } else {
                        times[i] = "0" + std::to_string(sum_spent_hours) + ":" + std::to_string(sum_spent_minutes);
                    }
                } else {
                    if (sum_spent_minutes < 10) {
                        times[i] = std::to_string(sum_spent_hours) + ":0" + std::to_string(sum_spent_minutes);
                    } else {
                        times[i] = std::to_string(sum_spent_hours) + ":" + std::to_string(sum_spent_minutes);
                    }
                }

                int hours_for_payment = spent_hours;
                if (spent_minutes != 0) {
                    hours_for_payment = hours_for_payment + 1;
                }
                earnings[i] = earnings[i] + (hours_for_payment * cost_per_hour);

                tables[i] = "empty";
                start_times[i] = "00:00";
                clients_in_club.erase(current);

                std::cout << end_time + " " << 11 << " " + client_name << std::endl;
                break;
            }
        }
    }
    std::cout << end_time << std::endl;
    for (int i = 0; i < table_count; i++) {
        std::cout << i + 1 << " " << earnings[i] << " " << times[i] << std::endl;
    }
}