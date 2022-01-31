//
// Created by Anton Ovcharenko on 18.01.2022.
//

#include "ShowAllLabelsStep.h"
#include "ui/Context.h"

ShowAllLabelsStep::ShowAllLabelsStep(const std::shared_ptr<AbstractPrinter> &printer) :
        PrinterStep(printer) {
}

std::unique_ptr<Action> ShowAllLabelsStep::genAction(Context &context) {
    std::ostringstream os;
    if (!context.tasks().empty()) {
        auto labels = context.tasks()[0].data().labels();
        for (int i = 0; i < labels.size(); ++i) {
            os << labels[i];
            if (i < labels.size()-1)
                os << ", ";
        }

        if (!labels.empty())
            os << "\n";

        printer()->print(os.str());
    }
    return std::unique_ptr<Action>(new DoNothingAction);
}

std::shared_ptr<Step> ShowAllLabelsStep::genNextStep(const ActionResult &result, const std::shared_ptr<Factory> &factory) {
    return processResult(*this, factory, result, "");

}
