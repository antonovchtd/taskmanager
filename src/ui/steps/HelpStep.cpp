//
// Created by Anton Ovcharenko on 14.01.2022.
//

#include "HelpStep.h"

HelpStep::HelpStep(const std::shared_ptr<AbstractReader> &reader,
                   const std::shared_ptr<AbstractPrinter> &printer) :
                   IOStep(reader, printer) {
}

std::unique_ptr<Action> HelpStep::genAction(Context &) {
    std::string filename{"../src/model/help.txt"};
    std::ifstream file(filename);
    std::ostringstream os;
    std::string line;
    if (file.is_open()) {
        while (getline(file, line))
            os << line << "\n";
        file.close();
    } else {
        os << "Failed to open file " + filename << std::endl;
    }
    printer()->print(os.str());
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> HelpStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");
}

