//
// Created by Anton O. on 12/18/21.
//

#include "ActionResult.h"

ActionResult::operator bool() const {
    return status == ActionResult::Status::SUCCESS;
}

std::string ActionResult::message() const {
    switch (status) {
//        case Status::
    }

}