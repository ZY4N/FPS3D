#include <collision.hpp>
/*
float Collision::signedDistTri(vf3 dir, const triangle& s, const triangle& t) {

    //----------/ check if moving apart /----------//

    if (t.normal.dot(dir) > -EPSILON)
        return FLT_MAX;

    //----------/ swap triangles if projection would be impossible /----------//

    const triangle* t1 = &s;
    const triangle* t2 = &t;

    if (fabs(t.normal.dot(dir)) < EPSILON) {
        t1 = &t;
        t2 = &s;
        dir.mult(-1);
    }

    //----------/ project t on s and check 2D vertex overlap /----------//

    float minSignedDist = FLT_MAX;

    const vf3 xCompS = vf3::sub(*(t1->vertices[1]), *(t1->vertices[0]));
    const vf3 yCompS = vf3::sub(*(t1->vertices[2]), *(t1->vertices[0]));

    vf3 pvec = dir.cross(yCompS);
    float invDet = 1.0f / xCompS.dot(pvec);

    for (size_t i = 0; i < 3; i++) {

        //----------/ check if point inside triangle /----------//

        const vf3 tvec = vf3::sub(*(t2->vertices[i]), *(t1->vertices[0]));
        const float u = tvec.dot(pvec) * invDet;
        if (u < 0 || u > 1)
            continue;

        const vf3 qvec = tvec.cross(xCompS);
        const float v = dir.dot(qvec) * invDet;
        if (v < 0 || u + v > 1)
            continue;

        //----------/ check if moving apart /----------//

        const float dist = yCompS.dot(qvec) * invDet;
        if(dist < EPSILON)
            minSignedDist = fmin(dist * dist, minSignedDist);
    }

    //----------/ project s on t and check 2D vertex overlap (save 2D coords) /----------//

    const vf3 xCompT = vf3::sub(*(t2->vertices[1]), *(t2->vertices[0]));
    const vf3 yCompT = vf3::sub(*(t2->vertices[2]), *(t2->vertices[0]));

    pvec = dir.cross(yCompT);
    invDet = 1.0f / xCompT.dot(pvec);

    vf2 sOnt2D[3];

    for (size_t i = 0; i < 3; i++) {

        //----------/ check if point inside triangle /----------//

        const vf3 tvec = vf3::sub(*(t1->vertices[i]), *(t2->vertices[0]));
        const vf3 qvec = tvec.cross(xCompT); 

        const float u = tvec.dot(pvec) * invDet;
        const float v = dir.dot(qvec) * invDet;

        //----------/ check if moving apart /----------//

        if(u >= 0 && v >= 0 && u + v <= 1) {
            const float dist = yCompT.dot(qvec) * invDet;
            if (dist > -EPSILON) {
                minSignedDist = fmin(dist * dist, minSignedDist);
            }
        }

        sOnt2D[i].x = u;
        sOnt2D[i].y = v;
    }

    //----------/ check 2D edge intersection (both projected on t) /----------//

    constexpr vf2 tOnt2D[3]{
        { 0, 0 },
        { 1, 0 },
        { 0, 1 }
    };

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            const vf2 a1 = sOnt2D[i];
            const vf2 a2 = sOnt2D[(i + 1) < 3 ? (i + 1) : 0];
            const vf2 b1 = tOnt2D[j];
            const vf2 b2 = tOnt2D[(j + 1) < 3 ? (j + 1) : 0];

            vf2 coords;
            if (intersectionLine(a1, a2, b1, b2, coords)) {
                vf3 p3Ds{
                    t1->vertices[i]->x + t1->edges[i].x * coords.x,
                    t1->vertices[i]->y + t1->edges[i].y * coords.x,
                    t1->vertices[i]->z + t1->edges[i].z * coords.x
                };
                vf3 p3Dt{
                    t2->vertices[j]->x + t2->edges[j].x * coords.y,
                    t2->vertices[j]->y + t2->edges[j].y * coords.y,
                    t2->vertices[j]->z + t2->edges[j].z * coords.y
                };

                vf3 st = vf3::sub(p3Dt, p3Ds);
                if(st.dot(dir) > -EPSILON)
                    minSignedDist = fmin(st.dot(st), minSignedDist);
            }
        }
    }

    return minSignedDist;
}

bool Collision::intersectionLine(const vf2& a1, const vf2& a2, const vf2& b1, const vf2& b2, vf2& result) {

    vf2 delta1 = vf2::sub(a2, a1);
    vf2 delta2 = vf2::sub(b2, b1);
    vf2 delta3 = vf2::sub(a1, b1);

    float d = delta1.x * delta2.y - delta2.x * delta1.y;

    if (d != 0) {
        float s = delta1.x * delta3.y - delta3.x * delta1.y;
        if ((s <= 0 && d < 0 && s >= d) || (s >= 0 && d > 0 && s <= d)) {
            float t = delta2.x * delta3.y - delta3.x * delta2.y;
            if ((t <= 0 && d < 0 && t > d) || (t >= 0 && d > 0 && t < d)) {
                result.x = t / d;
                result.y = s / d;
                return true;
            }
        }
    }
    return false;
}
*/