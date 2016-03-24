#include "dateutil.h"

#include <QDebug>

static string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
static string week_days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

DateUtil::DateUtil()
{

}

Date DateUtil::get_current_date() {
    return date_from_timestamp(static_cast <unsigned long> (time(NULL)));
}

string DateUtil::get_literal_month(int m) {
    if ((m < 1) || (m > 12))
        return string("");
    else
        return months[m-1];
}

string DateUtil::numeric2literal_day_of_week(int d) {
    if ((d < 1) || (d > 7))
        return string("");
    else
        return week_days[d-1];
}

int DateUtil::literal2numeric_day_of_week(string d) {
    int i = 0;
    for (i = 0; i < 7; i++) {
        if (d == week_days[i])
            return i+1;
    }
    return -1;
}

int DateUtil::get_days_in_month(int month, int year) {
    int numberOfDays;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        numberOfDays = 30;
    else if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            numberOfDays = 29;
        else
            numberOfDays = 28;
    }
    else
      numberOfDays = 31;
    return numberOfDays;
}

/* Assume to receive a valid timestamp */
Date DateUtil::date_from_timestamp(unsigned long timestamp) {
    const time_t t = static_cast<const time_t> (timestamp);
    struct tm *tm = localtime(&t);
    Date date(tm->tm_mday, tm->tm_wday ?: 7, tm->tm_mon + 1, tm->tm_year + 1900);
    //free(tm); segfault?
    return date;
}

Date DateUtil::get_first_day_of_month(Date &date) {
    int wday = date.getWeekDay() - (date.getMonthDay() % 7) + 1;
    //Normalization
    if (wday <= 0) wday += 7;
    else if (wday > 7) wday -= 7;
    return Date(1, wday, date.getMonth(), date.getYear());
}

Date DateUtil::get_last_day_of_month(Date &date) {
    Date first_day = get_first_day_of_month(date);
    int tot_days = get_days_in_month(date.getMonth(), date.getYear());
    int wday = (tot_days % 7 ?: 7) + first_day.getWeekDay() - 1;
    return Date(tot_days, wday, date.getMonth(), date.getYear());
}

//TODO write tests
//TODO attention: each month has a different number of days (e.g. 31 doesn't exist in february)
//Assume to get a valid time (i.e. no negative numbers, etc.)
Date DateUtil::increase_month(Date date) {
    Date last_day_curr_month = get_last_day_of_month(date);
    Date first_day_next_month;
    if (date.getMonth() < 12) {
        first_day_next_month = Date(1, (last_day_curr_month.getWeekDay() % 7) + 1, date.getMonth() + 1, date.getYear());
    } else { //Go to next year
        first_day_next_month = Date(1, (last_day_curr_month.getWeekDay() % 7) + 1, 1, date.getYear() + 1);
    }
    return Date(date.getMonthDay(), (date.getMonthDay() % 7 ?: 7) + first_day_next_month.getWeekDay() - 1, first_day_next_month.getMonth(), first_day_next_month.getYear());
}

//Assume to get a valid time (i.e. no negative numbers, etc.)
Date DateUtil::decrease_month(Date date) {
    Date first_day_curr_month = get_first_day_of_month(date);
    Date last_day_prev_month;
    if (date.getMonth() > 1) {
        last_day_prev_month = Date(get_days_in_month(date.getMonth() - 1, date.getYear()), first_day_curr_month.getWeekDay() - 1 ?: 7, date.getMonth() - 1, date.getYear());
    } else { //Go to previous year
        last_day_prev_month = Date(get_days_in_month(12, date.getYear() - 1), first_day_curr_month.getWeekDay() - 1 ?: 7, 12, date.getYear() - 1);
    }
    Date first_day_prev_month = get_first_day_of_month(last_day_prev_month);
    return Date(date.getMonthDay(), (date.getMonthDay() % 7 ?: 7) + first_day_prev_month.getWeekDay() - 1, last_day_prev_month.getMonth(), last_day_prev_month.getYear());
}
