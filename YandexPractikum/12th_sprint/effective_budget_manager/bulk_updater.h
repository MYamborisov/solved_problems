#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

struct BulkMoneyAdder {
    Day delta = {};
};

struct BulkTaxApplier {
    double factor = 1.0;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
            : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
            : tax_(tax) {
    }

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.factor *= other.tax_.factor;
        add_.delta.spent += other.add_.delta.spent;
        add_.delta.income= add_.delta.income* other.tax_.factor + other.add_.delta.income;
    }

    Day Collapse(Day origin, IndexSegment segment) const {
        return {origin.income * tax_.factor + add_.delta.income * static_cast<double>(segment.length()) ,
                origin.spent + add_.delta.spent * static_cast<double>(segment.length())};
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
};
