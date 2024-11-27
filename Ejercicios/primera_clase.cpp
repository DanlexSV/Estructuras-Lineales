#include <iostream>
using namespace std;

class S {
    private:
        int id;
    public:
        S(int n)
            : id{n}
        {
            cout << "Se ha creado el objeto #" << id << endl;
        }

        ~S()
        {
            cout << "Se destruye el objeto #" << id << endl;
        }
};

auto main() -> int {
    auto s1 = S{1};

    {
        auto s2 = S{2};
        auto s3 = S{3};
    }

    auto s4 = S{4};

    return 0;
}
