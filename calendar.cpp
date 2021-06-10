#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Date {
    long long Year;
    long long Month;
    long long Day;
};

#pragma region Functions
bool IsLeap(Date date) {
    if (!(date.Year % 4)) {
        if (!(date.Year % 100))
            return false;
        if (!(date.Year % 400))
            return true;
        return true;
    }
    return false;
}
int YearDaysCount(Date date) {
    return IsLeap(date) ? 366 : 365;
}
int MonthDaysCount(Date date) {
    if (date.Month == 2)
        return IsLeap(date) ? 29 : 28;
    else if (date.Month == 1 || date.Month == 3 || date.Month == 5 || date.Month == 7 || date.Month == 8 || date.Month == 10 || date.Month == 12)
        return 31;
    return 30;
}
int DaysFromJan(Date date) {
    int TotalDays = date.Day - 1;
    for (int i = 0; i < date.Month - 1; i++)
        TotalDays += MonthDaysCount(Date{ date.Year, i + 1, 1 });
    return TotalDays;
}
long long DaysFrom1st(Date date) {
    int FromJan = DaysFromJan(Date{ date.Year, date.Month, date.Day });
    return (--date.Year) / 4 - date.Year / 100 + date.Year / 400 + date.Year * 365 + FromJan;
}
long long DatesDeltaDays(Date date1, Date date2) {
    auto _1 = DaysFrom1st(date1) - DaysFrom1st(date2);
    return _1;
}
int NextDateDayOfWeek(Date date1, Date date2, int dayOfWeek) {
    int tmp = (dayOfWeek + DatesDeltaDays(date1, date2) % 7) % 7;
    return tmp >= 1 ? tmp : 7 + tmp;
}
#pragma endregion

Date today = { 2021, 1, 22 };
string Months[]{ "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
string Days[]{ "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

int main() {
    setlocale(LC_ALL, "ru");
    while (1) {
        try {
            system("cls");
            int year = 0;
            bool spaces = false;
            char c;
            cout << "Enter the year to display the calendar: ";
            while (c = cin.get(), c != '\n') {
                if (year == -1)
                    continue;
                if (spaces) {
                    if (c == ' ')
                        continue;
                    else if (!year)
                        spaces = false;
                    else
                        year = -1;
                }
                else if (c == ' ') {
                    spaces = true;
                    continue;
                }
                if (c >= '0' && c <= '9')
                    year = year == 0 ? c - '0' : year * 10 + c - '0';
                else
                    year = -1;
            }
            if (year <= 0)
                throw exception("Invalid input data type");
            Date newDate = { year, 1, 1 };
            int months[12][7][6] = { 0 };
            int _1stIndex = NextDateDayOfWeek(newDate, today, 5) - 1;
            for (int m = 0; m < 12; m++) {
                int day = 1;
                int week = 0;
                for (int i = 0; i < 7 && day <= MonthDaysCount(Date{ year, m + 1, 1 }); i++) {
                    if (!week && i < _1stIndex)
                        continue;
                    months[m][i][week] = day++;
                    if (i == 6) {
                        week++;
                        i = -1;
                    }
                }
                if (m != 11)
                    _1stIndex = NextDateDayOfWeek(Date{ year, m + 2, 1 }, today, 5) - 1;
            }
            system("cls");
            cout << "Calendar for " << year << " год\n\n";
            for (int j = 0; j < 12; j++) {
                cout << setw(26) << left << Months[j];
                if (!((j + 1) % 4)) {
                    cout << "\n";
                    for (int i = 0; i < 7; i++) {
                        cout << setw(3) << left << Days[i];
                        for (int m = j / 4 * 4; m < j + 1; m++) {
                            for (int z = 0; z < 6; z++)
                                cout << setw(3) << left << (months[m][i][z] ? to_string(months[m][i][z]) : "");
                            if (m != j)
                                cout << setw(0) << left << "     " << Days[i] + " ";
                        }
                        cout << "\n";
                    }
                    cout << "\n";
                }
            }
        }
        catch (exception ex) {
            cout << "Error during program execution: " << ex.what() << "\n\n";
            goto end;
        }
    end:
        system("pause");
        continue;
    }
}
