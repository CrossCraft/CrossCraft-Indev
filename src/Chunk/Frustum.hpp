#pragma once
#include <mathfu/vector.h>
#include <mathfu/matrix.h>

class Frustum
{
public:
    Frustum();

    bool IsBoxVisible(const mathfu::Vector<float, 3>& minp, const mathfu::Vector<float, 3>& maxp) const;

    inline static auto get() -> Frustum& {
        static Frustum frustum;
        return frustum;
    }

    inline static auto update() -> void {
        get().update_frustum();
    }

private:
    auto update_frustum() -> void;

    enum Planes
    {
        Left = 0,
        Right,
        Bottom,
        Top,
        Near,
        Far,
        Count,
        Combinations = Count * (Count - 1) / 2
    };

    template<Planes i, Planes j>
    struct ij2k
    {
        enum { k = i * (9 - i) / 2 + j - 1 };
    };

    template<Planes a, Planes b, Planes c>
    mathfu::Vector<float, 3> intersection(const mathfu::Vector<float, 3>* crosses) const;

    mathfu::Vector<float, 4>   m_planes[Count];
    mathfu::Vector<float, 3>   m_points[8];
};
