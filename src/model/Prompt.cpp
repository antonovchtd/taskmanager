//
// Created by Anton O. on 11/19/21.
//

#include <sstream>
#include <fstream>

#include "Prompt.h"

std::ostream & operator<<(std::ostream &os, const TaskID& id);

void Prompt::ask(std::ostream &os) {
    switch (state_) {
        case Prompt::State::EMPTY:
            os << " > ";
            break;
        case Prompt::State::TASK:
            os << "add Task > " << std::endl;
            break;
        case Prompt::State::SUBTASK:
            os << "add SubTask of " << curr_id_ << " > " << std::endl;
            break;
        case Prompt::State::EDIT:
            os << "edit Task " << curr_id_ << " > " << std::endl;
            break;
        case Prompt::State::LABEL:
            os << "add Label to " << curr_id_ << " > ";
            break;
    }
}

void Prompt::input(std::ostream &os, std::istream &is){
    std::string s;
    switch (state_) {
        case Prompt::State::EMPTY:
        case Prompt::State::LABEL:
            std::getline(is,instr_);
            break;
        case Prompt::State::TASK:
        case Prompt::State::SUBTASK:
        case Prompt::State::EDIT:
            os << "    enter title >> ";
            std::getline(is,title_);

            while (true){
                os << "    enter priority ([0]:NONE, [1]:LOW, [2]:MEDIUM, [3]:HIGH) >> ";
                (std::getline(is,s));
                int p = std::atoi(s.c_str());
                if (p >= static_cast<int>(Task::Priority::NONE) &&
                    p <= static_cast<int>(Task::Priority::HIGH)) {
                    priority_ = static_cast<Task::Priority>(p);
                    break;
                }
                else{
                    os << "Wrong priority option. Try again.\n";
                }
            }

            os << "    enter due date (format: 'in dd:hh:mm' or 'dd/mm/(yy)yy (hh:mm)') >> ";
            std::getline(is,due_date_);
            break;
    }
}

void Prompt::act(std::ostream &os, std::istream &is, TaskManager & tm) {
    std::stringstream ss;
    std::string s;
    switch (state_) {
        case Prompt::State::EMPTY:
            ss.str(instr_);
            ss >> s;
            if (s == "add"){
                state_ = Prompt::State::TASK;
            }
            else if (s == "subtask"){
                ss >> curr_id_;
                if (tm.Validate(curr_id_))
                    state_ = Prompt::State::SUBTASK;
                else
                    os << "Wrong ID " << curr_id_ << "! Try again...\n";
            }
            else if (s == "edit"){
                ss >> curr_id_;
                if (tm.Validate(curr_id_))
                    state_ = Prompt::State::EDIT;
                else
                    os << "Wrong ID " << curr_id_ << "! Try again...\n";
            }
            else if (s == "delete"){
                ss >> curr_id_;
                if (!tm[curr_id_].second.children().empty()){
                    os << "Task " << curr_id_ << " has undeleted subtasks. Please confirm the deletion y/n: ";
                    std::string reply;
                    while (std::getline(is, reply)) {
                        if (reply == "y" || reply == "yes") {
                            // delete children
                            for (const auto & ch : tm[curr_id_].second.children())
                                tm.Delete(ch);
                            break; // continue to Delete() call
                        } else if (reply == "n" || reply == "no") {
                            os << "Aborting...\n";
                            return;
                        }
                        else{
                            os << "Wrong reply, try again. y/n: ";
                        }
                    }
                }

                tm.Delete(curr_id_);
            }
            else if (s == "complete"){
                ss >> curr_id_;
                tm.Complete(curr_id_);
            }
            else if (s == "show"){
                std::string label;
                ss >> label;
                if (label.empty())
                    tm.Show(os);
                else
                    tm.Show(os, label);
            }
            else if (s == "quit"){
                quit_flag_= true;
            }
            else if (s == "label"){
                ss >> curr_id_;
                state_ = Prompt::State::LABEL;
            }
            else if (s == "help"){
                std::ifstream f("../src/model/help.txt");
                if (f.is_open()) {
                    os << f.rdbuf();
                    f.close();
                }
            }
            else if (!s.empty()){
                os << "Wrong command, try again.\n";
            }
            break;
        case Prompt::State::TASK:
            curr_id_ = tm.Add(Task::Create(title_, priority_, due_date_, false));
            os << "Added Task with ID " << curr_id_ << "\n";
            state_ = State::EMPTY;
            break;
        case Prompt::State::SUBTASK:
            curr_id_ = tm.Add(Task::Create(title_, priority_, due_date_, false), TaskID(curr_id_));
            os << "Added Subtask with ID " << curr_id_ << "\n";
            state_ = State::EMPTY;
            break;
        case Prompt::State::EDIT:
            tm.Edit(curr_id_, Task::Create(title_, priority_, due_date_, false));
            state_ = State::EMPTY;
            break;
        case Prompt::State::LABEL:
            tm[curr_id_].second.SetLabel(instr_);
            state_ = State::EMPTY;
            break;
    }

}