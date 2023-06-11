#include <Entity/Entity.hpp>
#include <CC/item.h>

namespace CrossCraft {

    class ItemEntity : public Entity {
    public:
        ItemEntity();

        ~ItemEntity() override;

        auto update(double dt) -> void override;

        auto draw() -> void override;

        float lifetimer;
        ItemData *data;
    };
}