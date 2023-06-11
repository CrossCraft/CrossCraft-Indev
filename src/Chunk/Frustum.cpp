#include <Chunk/Frustum.hpp>
#include <Rendering/RenderContext.hpp>

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Rendering;

Frustum::Frustum() {
    update_frustum();
}

auto Frustum::update_frustum() -> void {
    auto m = RenderContext::get().get_projection_matrix() * RenderContext::get().get_view_matrix();
    m = m.Transpose();
    m_planes[Left]   = m.GetColumn(3) + m.GetColumn(0);
    m_planes[Right]  = m.GetColumn(3) - m.GetColumn(0);
    m_planes[Bottom] = m.GetColumn(3) + m.GetColumn(1);
    m_planes[Top]    = m.GetColumn(3) - m.GetColumn(1);
    m_planes[Near]   = m.GetColumn(3) + m.GetColumn(2);
    m_planes[Far]    = m.GetColumn(3) - m.GetColumn(2);

    mathfu::Vector<float, 3> crosses[Combinations] = {
            mathfu::CrossProductHelper((m_planes[Left]).xyz(),   (m_planes[Right]).xyz()),
            mathfu::CrossProductHelper((m_planes[Left]).xyz(),   (m_planes[Bottom]).xyz()),
            mathfu::CrossProductHelper((m_planes[Left]).xyz(),   (m_planes[Top]).xyz()),
            mathfu::CrossProductHelper((m_planes[Left]).xyz(),   (m_planes[Near]).xyz()),
            mathfu::CrossProductHelper((m_planes[Left]).xyz(),   (m_planes[Far]).xyz()),
            mathfu::CrossProductHelper((m_planes[Right]).xyz(),  (m_planes[Bottom]).xyz()),
            mathfu::CrossProductHelper((m_planes[Right]).xyz(),  (m_planes[Top]).xyz()),
            mathfu::CrossProductHelper((m_planes[Right]).xyz(),  (m_planes[Near]).xyz()),
            mathfu::CrossProductHelper((m_planes[Right]).xyz(),  (m_planes[Far]).xyz()),
            mathfu::CrossProductHelper((m_planes[Bottom]).xyz(), (m_planes[Top]).xyz()),
            mathfu::CrossProductHelper((m_planes[Bottom]).xyz(), (m_planes[Near]).xyz()),
            mathfu::CrossProductHelper((m_planes[Bottom]).xyz(), (m_planes[Far]).xyz()),
            mathfu::CrossProductHelper((m_planes[Top]).xyz(),    (m_planes[Near]).xyz()),
            mathfu::CrossProductHelper((m_planes[Top]).xyz(),    (m_planes[Far]).xyz()),
            mathfu::CrossProductHelper((m_planes[Near]).xyz(),   (m_planes[Far]).xyz())
    };

    m_points[0] = intersection<Left,  Bottom, Near>(crosses);
    m_points[1] = intersection<Left,  Top,    Near>(crosses);
    m_points[2] = intersection<Right, Bottom, Near>(crosses);
    m_points[3] = intersection<Right, Top,    Near>(crosses);
    m_points[4] = intersection<Left,  Bottom, Far>(crosses);
    m_points[5] = intersection<Left,  Top,    Far>(crosses);
    m_points[6] = intersection<Right, Bottom, Far>(crosses);
    m_points[7] = intersection<Right, Top,    Far>(crosses);
}

// http://iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
bool Frustum::IsBoxVisible(const mathfu::Vector<float ,3>& minp, const mathfu::Vector<float ,3>& maxp) const {
    // check box outside/inside of frustum
    for (int i = 0; i < Count; i++)
    {
        if ((mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(minp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(maxp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(minp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(maxp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(minp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(maxp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(minp.x, maxp.y, maxp.z, 1.0f)) < 0.0) &&
            (mathfu::DotProductHelper(m_planes[i], mathfu::Vector<float, 4>(maxp.x, maxp.y, maxp.z, 1.0f)) < 0.0))
        {
            return false;
        }
    }

    // check frustum outside/inside box
    int out;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].x > maxp.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].x < minp.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].y > maxp.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].y < minp.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].z > maxp.z) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i<8; i++) out += ((m_points[i].z < minp.z) ? 1 : 0); if (out == 8) return false;

    return true;
}

template<Frustum::Planes a, Frustum::Planes b, Frustum::Planes c>
mathfu::Vector<float, 3> Frustum::intersection(const mathfu::Vector<float, 3>* crosses) const
{
    float D = mathfu::DotProductHelper(m_planes[a].xyz(), crosses[ij2k<b, c>::k]);
    mathfu::Matrix<float, 3, 3> mat;
    mat.data_[0] = crosses[ij2k<b, c>::k];
    mat.data_[1] = -crosses[ij2k<a, c>::k];
    mat.data_[2] = crosses[ij2k<a, b>::k];

    mathfu::Vector<float, 3> res = mat * mathfu::Vector<float, 3>(m_planes[a].w, m_planes[b].w, m_planes[c].w);
    return res * (-1.0f / D);
}