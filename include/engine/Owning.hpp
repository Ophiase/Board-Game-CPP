#pragma once

enum Player {Red, Blue};

class Owning {
    public:
        virtual ~Owning() = default;
        Player owner;
};
