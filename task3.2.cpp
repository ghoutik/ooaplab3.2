
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <memory>
#include <Windows.h>

using namespace std;


class Customer {
public:
    string surname;
    string name;
    string phone;

    void input() {
        cout << "\n--- Дані покупця ---\n";

        cout << "Прізвище: ";
        getline(cin, surname);

        cout << "Ім'я: ";
        getline(cin, name);

        cout << "Номер телефону: ";
        getline(cin, phone);
    }
};

class Car {
public:
    string brand;
    string model;
    string equipment;
    double price;

    void input() {
        cout << "\n--- Дані автомобіля ---\n";

        cout << "Марка: ";
        getline(cin, brand);

        cout << "Модель: ";
        getline(cin, model);

        cout << "Комплектація: ";
        getline(cin, equipment);

        cout << "Початкова ціна: ";
        cin >> price;
        cin.ignore();
    }
};


class Purchase {
public:
    virtual unique_ptr<Purchase> clone() const = 0;

    virtual double calculatePrice(double price) const = 0;

    virtual string getInsurance() const = 0;

    virtual string getWarranty() const = 0;

    virtual string getType() const = 0;

    virtual ~Purchase() {}
};


class CashPurchase : public Purchase {
public:
    unique_ptr<Purchase> clone() const override {
        return make_unique<CashPurchase>(*this);
    }

    double calculatePrice(double price) const override {
        return price * 0.95;
    }

    string getInsurance() const override {
        return "Повне страхування";
    }

    string getWarranty() const override {
        return "Гарантія 5 років";
    }

    string getType() const override {
        return "Повна оплата";
    }
};

class CreditPurchase : public Purchase {
public:
    unique_ptr<Purchase> clone() const override {
        return make_unique<CreditPurchase>(*this);
    }

    double calculatePrice(double price) const override {
        return price * 1.10;
    }

    string getInsurance() const override {
        return "Страхування КАСКО";
    }

    string getWarranty() const override {
        return "Гарантія 3 роки";
    }

    string getType() const override {
        return "Кредит";
    }
};

class InstallmentPurchase : public Purchase {
public:
    unique_ptr<Purchase> clone() const override {
        return make_unique<InstallmentPurchase>(*this);
    }

    double calculatePrice(double price) const override {
        return price * 1.05;
    }

    string getInsurance() const override {
        return "Часткове страхування";
    }

    string getWarranty() const override {
        return "Гарантія 2 роки";
    }

    string getType() const override {
        return "Розтермінування";
    }
};

class Database {
public:
    static void save(
        const Customer& customer,
        const Car& car,
        const Purchase& purchase,
        double finalPrice
    ) {
        ofstream file("database.txt", ios::app);

        if (!file.is_open()) {
            cout << "Помилка відкриття файлу!\n";
            return;
        }

        file << fixed << setprecision(2);

        file << "================================\n";
        file << "Інформація про продаж\n";

        file << "Покупець: "
            << customer.surname << " "
            << customer.name << "\n";

        file << "Телефон: " << customer.phone << "\n";

        file << "Марка автомобіля: " << car.brand << "\n";
        file << "Модель: " << car.model << "\n";
        file << "Комплектація: " << car.equipment << "\n";

        file << "Початкова ціна: " << car.price << " грн\n";

        file << "Спосіб оплати: "
            << purchase.getType() << "\n";

        file << "Кінцева ціна: " << finalPrice << " грн\n";

        file << "Страхування: "
            << purchase.getInsurance() << "\n";

        file << "Гарантія: "
            << purchase.getWarranty() << "\n";

        file << "================================\n\n";

        file.close();

        cout << "\nІнформацію успішно збережено!\n";
    }
};


int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;

    cout << "====================================\n";
    cout << "       Автосалон\n";
    cout << "====================================\n";

    cout << "\nОберіть спосіб придбання автомобіля:\n";
    cout << "1. Повна оплата\n";
    cout << "2. Кредит\n";
    cout << "3. Розтермінування\n";

    cout << "\nВаш вибір: ";
    cin >> choice;
    cin.ignore();

    CashPurchase cashPrototype;
    CreditPurchase creditPrototype;
    InstallmentPurchase installmentPrototype;

    const Purchase* prototype = nullptr;

    switch (choice) {
    case 1:
        prototype = &cashPrototype;
        break;

    case 2:
        prototype = &creditPrototype;
        break;

    case 3:
        prototype = &installmentPrototype;
        break;

    default:
        cout << "Неправильний вибір!\n";
        return 0;
    }

    unique_ptr<Purchase> purchase = prototype->clone();

    Customer customer;
    Car car;

    customer.input();
    car.input();

    if (car.price <= 0) {
        cout << "Ціна автомобіля повинна бути більшою за 0!\n";
        return 0;
    }

    double finalPrice = purchase->calculatePrice(car.price);

    cout << fixed << setprecision(2);

    cout << "\n====================================\n";
    cout << "         Результат\n";
    cout << "====================================\n";

    cout << "Покупець: "
        << customer.surname << " "
        << customer.name << "\n";

    cout << "Автомобіль: "
        << car.brand << " " << car.model << "\n";

    cout << "Комплектація: " << car.equipment << "\n";

    cout << "Початкова ціна: "
        << car.price << " грн\n";

    cout << "Спосіб оплати: "
        << purchase->getType() << "\n";

    cout << "Кінцева ціна: "
        << finalPrice << " грн\n";

    cout << "Страхування: "
        << purchase->getInsurance() << "\n";

    cout << "Гарантія: "
        << purchase->getWarranty() << "\n";

    cout << "====================================\n";

    Database::save(customer, car, *purchase, finalPrice);

    return 0;
}
