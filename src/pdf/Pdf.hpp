//
// Created by zhang on 11/20/19.
//

#ifndef MONTE_CARLO_RAY_TRACER_PDF_HPP
#define MONTE_CARLO_RAY_TRACER_PDF_HPP

#include "../common/Typing.hpp"

class PDF {
    public:
    [[nodiscard]] virtual std::pair<Direction, float> GenerateWithPDF() const = 0;
};

#endif  //MONTE_CARLO_RAY_TRACER_PDF_HPP
