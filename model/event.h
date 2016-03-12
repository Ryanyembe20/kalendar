#ifndef EVENT_H
#define EVENT_H

using namespace std;

class Event
{
private:
    int id;
    string name;
    string description;
    string category;
    unsigned long start;
    unsigned long end;

public:
    Event(int id, string name, string description, string category, unsigned long start, unsigned long end) {
        this->name = name;
        this->description = description;
        this->category = category;
        if (id == 0)
            this->id = static_cast<int> (hash<string>()(this->name + this->description + this->category));
        else
            this->id = id;
        this->start = start;
        this->end = end;
    }

    ~Event() {
    }

    int getId() { return id; }
    string getName() { return name; }
    string getDescription() { return description; }
    string getCategory() { return category; }
    unsigned long getStart() { return start; }
    unsigned long getEnd() { return end; }

    bool equals(Event &e) {
        return (this->id == e.getId()) &&
               (this->start == e.getStart()) &&
               (this->end == e.getEnd());
    }

    int hashcode() {
        return this->id + static_cast<int> ((this->start / 1000) + (this->end - this->start));
    }
};

#endif // EVENT_H
