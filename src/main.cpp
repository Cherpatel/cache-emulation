#include "../include/ram.hpp"
#include "../include/func.hpp"

// caches[] - массив кэшей (указателей)
// cache[] - массив ячеек
// unit[] - массив линий
// data[] - массив double

int main() {
    // Код вставлять сюда

    double w = 7;
    f(w);

    //

    for (int i = 0; i < caches.size(); i++) {
        printf("Cache L%d:\n", i + 1);
        caches[i] -> printStat();
        delete caches[i];
    }

    return 0;
}
