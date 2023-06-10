#include <Entity/Entity.hpp>
#include <CC/item.h>

namespace CrossCraft {

    class ItemEntity : public Entity {
        public:
        ItemEntity();
        virtual ~ItemEntity();

        virtual auto update(double dt) -> void override;
        virtual auto draw() -> void override;

        float lifetimer;
        ItemData* data;
    };
}