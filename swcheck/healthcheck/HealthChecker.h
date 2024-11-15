//
// Created by Отинов Илья on 15.11.2024.
//

#ifndef SWCHECK_HEALTHCHECKER_H
#define SWCHECK_HEALTHCHECKER_H

class HealthChecker {
public:
    virtual bool run() const = 0;
};

#endif //SWCHECK_HEALTHCHECKER_H
