#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

// create commponent to store capasitor and resistor data
struct Component
{
    double value;

    Component(double val) : value(val) {}
};

// calculate resistors as a series set
double series_resistor_total(const vector<Component> &components)
{
    double total = 0.0;
    for (const Component &component : components)
    {
        total += component.value;
    }
    return total;
}

// calculate resistors as a parallel set
double parallel_resistor_total(const vector<Component> &components)
{
    double total_inverse = 0.0;
    for (const Component &component : components)
    {
        total_inverse += 1.0 / component.value;
    }
    return 1.0 / total_inverse;
}

// calculate capacitors as a parallel set
double parallel_capacitor_total(const vector<Component> &components)
{
    double total = 0.0;
    for (const Component &component : components)
    {
        total += component.value;
    }
    return total;
}

// calculate capacitors as a series set
double series_capacitor_total(const vector<Component> &components)
{
    double total_inverse = 0.0;
    for (const Component &component : components)
    {
        total_inverse += 1.0 / component.value;
    }
    return 1.0 / total_inverse;
}

// calculate current
double current(double voltage, double resistance)
{
    return voltage / resistance;
}

// convert values to engineering notation
// round values to two decimal points
string engineering_notation(double value, string unit)
{
    const char prefixesNegative[] = {' ', 'm', 'u', 'n', 'p'};
    const char prefixesPositive[] = {' ', 'k', 'M', 'G', 'T'};
    int exponent = 0;
    char prefix = ' ';

    if (value >= 1000.0)
    {
        while (value >= 1000.0 && exponent < 8)
        {
            value /= 1000.0;
            exponent++;
        }
        prefix = prefixesPositive[exponent];
    }
    else if (value <= (1.0 / 1000.0))
    {
        while (value <= (1.0 / 1000.0) && exponent < 8)
        {
            value *= 1000.0;
            exponent++;
        }
        prefix = prefixesNegative[exponent];
    }

    // Round 'number' to two decimal places using stringstream
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    string roundedNumber;
    stream >> roundedNumber;

    return roundedNumber + " " + prefix + unit;
}

// calculate rc time constant
double rc_time_constant(double resistance, double capacitance)
{
    return resistance * capacitance;
}

// calculate total resistance
double calculate_total_resistance(const vector<Component> &resistors)
{
    double total_resistance = 0.0;

    for (const Component &component : resistors)
    {
        total_resistance += component.value;
    }
    return total_resistance;
}

// calculate total capacitance
double calculate_total_capacitance(const vector<Component> &capacitors)
{
    double total_capacitance = 0.0;

    for (const Component &component : capacitors)
    {
        total_capacitance += component.value;
    }
    return total_capacitance;
}

// display rc time constant
void display_rc_time_constant(const vector<Component> &resistors, const vector<Component> &capacitors)
{
    double total_resistance = calculate_total_resistance(resistors);
    double total_capacitance = calculate_total_capacitance(capacitors);
    cout << "RC Time Constant : " << engineering_notation(rc_time_constant(total_resistance, total_capacitance), "s") << endl;
}

// Function to calculate charging time for a capacitor
double calculateChargingTime(double targetVoltage, double finalVoltage, double resistance, double capacitance)
{
    if (targetVoltage <= 0.0 || resistance <= 0.0 || capacitance <= 0.0)
    {
        std::cerr << "Ensure targetVoltage, resistance, and capacitance are positive." << std::endl;
        return -1.0; // Error value
    }

    // Calculate the charging time
    double initialVoltage = 0.0; // Initial voltage (discharged capacitor)
    double chargingTime = -resistance * capacitance * std::log((targetVoltage - finalVoltage) / (initialVoltage - finalVoltage));

    return chargingTime;
}

// Function to calculate discharging time for a capacitor
double calculateDischargingTime(double targetVoltage, double initialVoltage, double resistance, double capacitance)
{
    if (initialVoltage <= 0.0 || resistance <= 0.0 || capacitance <= 0.0)
    {
        std::cerr << "Ensure initialVoltage, resistance, and capacitance are positive." << std::endl;
        return -1.0; // Error value
    }

    // Calculate the discharging time
    double finalVoltage = 0.0; // Final voltage (fully discharged capacitor)
    double dischargingTime = -resistance * capacitance * std::log((targetVoltage - finalVoltage) / (initialVoltage - finalVoltage));

    return dischargingTime;
}

// Function to calculate charged voltage for a capacitor for a desired time
double calculateChargedVoltage(double finalVoltage, double time, double resistance, double capacitance)
{
    if (finalVoltage <= 0.0 || resistance <= 0.0 || capacitance <= 0.0)
    {
        std::cerr << "Ensure targetVoltage, resistance, and capacitance are positive." << std::endl;
        return -1.0; // Error value
    }

    // Calculate the charging time
    double initialVoltage = 0.0; // Initial voltage (discharged capacitor)
    double chargedVoltage = finalVoltage + (initialVoltage - finalVoltage) * std::exp(-time / (resistance * capacitance));

    return chargedVoltage;
}

// Function to calculate discharged voltage for a capacitor for a desired time
double calculateDischargedVoltage(double initialVoltage, double time, double resistance, double capacitance)
{
    if (initialVoltage <= 0.0 || resistance <= 0.0 || capacitance <= 0.0)
    {
        std::cerr << "Ensure initialVoltage, resistance, and capacitance are positive." << std::endl;
        return -1.0; // Error value
    }

    // Calculate the discharging time
    double finalVoltage = 0.0; // Final voltage (fully discharged capacitor)
    double dischargedVoltage = finalVoltage + (initialVoltage - finalVoltage) * std::exp(-time / (resistance * capacitance));

    return dischargedVoltage;
}

// display charge times
void display_charge_times(double voltage, const vector<Component> &resistors, const vector<Component> &capacitors)
{
    double total_resistance = calculate_total_resistance(resistors);
    double total_capacitance = calculate_total_capacitance(capacitors);

    // Calculate and display times and voltage levels for charge levels
    double chargeLevels[] = {0.01, 0.15, 0.35, 0.50, 0.65, 0.85, 0.99};
    cout << "\nCharging Times and Voltage Levels:" << endl;
    for (double level : chargeLevels)
    {
        double chargingTime = calculateChargingTime(level * voltage, voltage, total_resistance, total_capacitance);
        if (chargingTime != -1)
        {
            double voltageAtChargeLevel = voltage * (1 - exp(-chargingTime / (total_resistance * total_capacitance)));
            cout << "Charge Level: " << level * 100 << "%\tTime: " << engineering_notation(chargingTime, "s") << " \tVoltage: " << engineering_notation(voltageAtChargeLevel, "V") << endl;
        }
        else
        {
            break;
        }
    }

    // Calculate and display times and voltage levels for discharge levels
    cout << "\nDischarging Times and Voltage Levels:" << endl;
    for (double level : chargeLevels)
    {
        double dischargingTime = calculateDischargingTime(level * voltage, voltage, total_resistance, total_capacitance);
        if (dischargingTime != -1)
        {
            double voltageAtDischargeLevel = voltage * exp(-dischargingTime / (total_resistance * total_capacitance));
            cout << "Discharge Level: " << level * 100 << "%\tTime: " << engineering_notation(dischargingTime, "s") << " \tVoltage: " << engineering_notation(voltageAtDischargeLevel, "V") << endl;
        }
        else
        {
            break;
        }
    }
}

// display capacitor voltage for desired time
void display_voltage_for_time(double voltage, double time, const vector<Component> &resistors, const vector<Component> &capacitors)
{
    double total_resistance = calculate_total_resistance(resistors);
    double total_capacitance = calculate_total_capacitance(capacitors);

    // Calculate and display voltage level for desired time
    double voltageAtChargedTime = calculateChargedVoltage(voltage, time, total_resistance, total_capacitance);
    if (voltageAtChargedTime != -1)
    {
        cout << "\nCharging Voltage Level for " << time << "s: " << engineering_notation(voltageAtChargedTime, "V") << endl;
    }

    // Calculate and display times and voltage levels for discharge levels
    double voltageAtDischargedTime = calculateDischargedVoltage(voltage, time, total_resistance, total_capacitance);
    if (voltageAtChargedTime != -1)
    {
        cout << "Discharging Voltage Level for " << time << "s: " << engineering_notation(voltageAtDischargedTime, "V") << endl;
    }
}

// save data on a '.csv' file
void save_configuration(double voltage, const vector<Component> &resistors, const vector<Component> &capacitors)
{
    string filename = "Capacitor Configurations.csv";
    // Attempt to open the file for reading
    std::ifstream file(filename);

    // if file is already created
    if (file.is_open())
    {
        ofstream configFile(filename, ios::app);
        if (!configFile.is_open())
        {
            cerr << "Error: Unable to open configuration file." << endl;
            return;
        }
        double total_resistance = calculate_total_resistance(resistors);
        double total_capacitance = calculate_total_capacitance(capacitors);
        double rc_constant = rc_time_constant(total_resistance, total_capacitance);
        configFile << total_resistance << "," << total_capacitance << "," << rc_constant << ",";

        // Calculate and display times and voltage levels for charge levels
        double chargeLevels[] = {0.01, 0.15, 0.35, 0.50, 0.65, 0.85, 0.99};
        for (double level : chargeLevels)
        {
            double chargingTime = calculateChargingTime(level * voltage, voltage, total_resistance, total_capacitance);
            if (chargingTime != -1.0)
            {
                double voltageAtChargeLevel = voltage * (1 - exp(-chargingTime / (total_resistance * total_capacitance)));
                configFile << "Time: " << engineering_notation(chargingTime, "s") << " | "
                           << "Voltage: " << engineering_notation(voltageAtChargeLevel, "V") << ",";
            }
            else
            {
                cout << "Error saving: times and voltage levels for charge levels" << endl;
                break;
            }
        }
        configFile << endl;
        configFile.close();
        cout << "saved!" << endl;
    }
    // if file is not already created
    else
    {
        ofstream configFile(filename, ios::app);
        if (!configFile.is_open())
        {
            cerr << "Error: Unable to open configuration file." << endl;
            return;
        }
        // column headers
        configFile << "Total Resistance"
                   << ","
                   << "Total Capacitance"
                   << ","
                   << "RC Constant"
                   << ","
                   << "1%"
                   << ","
                   << "15%"
                   << ","
                   << "35%"
                   << ","
                   << "50%"
                   << ","
                   << "65%"
                   << ","
                   << "85%"
                   << ","
                   << "99%" << endl;
        // data
        double total_resistance = calculate_total_resistance(resistors);
        double total_capacitance = calculate_total_capacitance(capacitors);
        double rc_constant = rc_time_constant(total_resistance, total_capacitance);
        configFile << total_resistance << "," << total_capacitance << "," << rc_constant << ",";

        // Calculate and display times and voltage levels for charge levels
        double chargeLevels[] = {0.01, 0.15, 0.35, 0.50, 0.65, 0.85, 0.99};
        for (double level : chargeLevels)
        {
            double chargingTime = calculateChargingTime(level * voltage, voltage, total_resistance, total_capacitance);
            if (chargingTime != -1.0)
            {
                double voltageAtChargeLevel = voltage * (1 - exp(-chargingTime / (total_resistance * total_capacitance)));
                configFile << "Time: " << engineering_notation(chargingTime, "s") << ","
                           << "Voltage: " << engineering_notation(voltageAtChargeLevel, "V");
            }
            else
            {
                cout << "Error saving: times and voltage levels for charge levels" << endl;
                break;
            }
        }
        configFile << endl;
        configFile.close();
        cout << "saved!" << endl;
    }
}

// clear old data sets
void clear_configuration(vector<Component> &resistors, vector<Component> &capacitors)
{
    resistors.clear();
    capacitors.clear();
}

// display help
void display_help()
{
    cout << "\nYou can enter different sets of resistors and capacitors at different times. All of these sets are taken to do calculations by the program.";
    cout << "\nIf you want to clear previose data, you can simply enter 'c'.";
    cout << "\nBefore you clear the previouse data, you must save data to a '.csv' file. If not data will be lost";
    cout << "\nPress 'e' to save data!";
}

int main()
{
    cout << "Turing Moore Engineering Capacitor Simulator 1.0" << endl;
    string name = "Your Name";                  // Replace with your name
    string student_id = "123456789";            // Replace with your student ID
    string due_date = "dd/mm/yyyy";             // Replace with the due date
    string desired_level = "Desired Level: HD"; // Replace with your desired level

    cout << "Name: " << name << endl;
    cout << "Student ID: " << student_id << endl;
    cout << "Due Date: " << due_date << endl;
    cout << desired_level << endl;

    // store resistor and capacitor values
    vector<Component> resistors;
    vector<Component> capacitors;

    while (true)
    {
        cout << "\n\nSelect an option:" << endl;
        cout << "a - Calculate with resistors" << endl;
        cout << "b - Calculate with capacitors" << endl;
        cout << "c - Show the times and voltage levels for the capacitor to reach different levels" << endl;
        cout << "d - Clear all values" << endl;
        cout << "e - Save data to a '.csv' file" << endl;
        cout << "f - Exit" << endl;
        cout << "---------------------------------------------------------------------------------" << endl;
        cout << "Press 'h' for help" << endl;
        char option;
        cin >> option;

        switch (option)
        {
        case 'a':
        {
            // Handle resistor calculations
            char choice;
            vector<Component> resistors_temp; // Temporary vector to store resistor values

            cout << "\nEnter voltage value (in volts): ";
            double voltage_value;
            cin >> voltage_value;

            do
            {
                cout << "\nEnter resistor value (in ohms): ";
                double resistor_value;
                cin >> resistor_value;
                resistors_temp.push_back(Component(resistor_value));

                cout << "Add another resistor? (y/n): ";
                cin >> choice;
            } while (choice == 'y' || choice == 'Y');

            cout << "\nEnter configuration (s for series, p for parallel): ";
            cin >> choice;

            if (choice == 's' || choice == 'S')
            {
                // calculate total resister value as series and store
                double r_total = series_resistor_total(resistors_temp);
                resistors.push_back(Component(r_total));
            }
            else if (choice == 'p' || choice == 'P')
            {
                // calculate total resister value as parallel and store
                double r_total = parallel_resistor_total(resistors_temp);
                resistors.push_back(Component(r_total));
            }
            else if (choice == 'h' || choice == 'H')
            {
                display_help();
            }

            // Display the total resistance in engineering notation
            cout << "\nTotal Resistance: " << engineering_notation(resistors.back().value, "ohms") << endl;
            // Display the current in engineering notation
            cout << "Current: " << engineering_notation(current(voltage_value, resistors.back().value), "A") << endl;

            display_rc_time_constant(resistors, capacitors);
            break;
        }
        case 'b':
        {
            // Handle capacitor calculations
            char choice;
            vector<Component> capacitors_temp; // Temporary vector to store capacitor values

            do
            {
                cout << "\nEnter capacitor value (in farads): ";
                double capacitor_value;
                cin >> capacitor_value;
                // store current value
                capacitors_temp.push_back(Component(capacitor_value));

                cout << "Add another capacitor? (y/n): ";
                cin >> choice;
            } while (choice == 'y' || choice == 'Y');

            cout << "\nEnter configuration (s for series, p for parallel): ";
            cin >> choice;

            if (choice == 's' || choice == 'S')
            {
                // calculate total capacitor value as series and store
                double c_total = series_capacitor_total(capacitors_temp);
                capacitors.push_back(Component(c_total));
            }
            else if (choice == 'p' || choice == 'P')
            {
                // calculate total capacitor value as parallel and store
                double c_total = parallel_capacitor_total(capacitors_temp);
                capacitors.push_back(Component(c_total));
            }
            else if (choice == 'h' || choice == 'H')
            {
                display_help();
            }

            // Display the total capacitance in engineering notation
            cout << "\nTotal Capacitance: " << engineering_notation(capacitors.back().value, "F") << endl;

            display_rc_time_constant(resistors, capacitors);
            break;
        }
        case 'd':
        {
            // Clear all configurations
            clear_configuration(resistors, capacitors);
            cout << "\nCleared old data!";
            break;
        }
        case 'c':
        {
            cout << "\nYou choosed option 4: ";
            char choice;
            cout << "\nEnter 'v' to specify a desired voltage or 't' for a desired time: ";
            cin >> choice;

            if (choice == 'v' || choice == 'V')
            {
                // display charge times by inputing desired voltage
                double voltage;
                cout << "Enter desired voltage (in volts): ";
                cin >> voltage;
                display_charge_times(voltage, resistors, capacitors);
            }
            else if (choice == 't' || choice == 'T')
            {
                // display charge level by inputing desired time and battery voltage
                double voltage;
                cout << "Enter battery voltage (in volts): ";
                cin >> voltage;
                double time;
                cout << "Enter desired time (in seconds): ";
                cin >> time;
                display_voltage_for_time(voltage, time, resistors, capacitors);
            }
            else if (choice == 'h' || choice == 'H')
            {
                display_help();
            }
            break;
        }
        case 'e':
        {
            // get battery voltage to save charge and discharge times for different valtage levels
            double voltage;
            cout << "Enter desired voltage (in volts) to save charge and discharge times for 1%, 15%, 35%, 50%, 65%, 85% and 99%: ";
            cin >> voltage;
            // Save the configuration to the CSV file
            save_configuration(voltage, resistors, capacitors);
            break;
        }
        case 'f':
        {
            // Exit the program
            cout << "\nYou exited the program";
            return 0;
        }
        case 'h':
        {
            // display help
            display_help();
            break;
        }
        default:
        {
            // show error id choice in invalid
            cout << "Invalid option. Please select a valid option." << endl;
            break;
        }
        }
    }

    return 0;
}
