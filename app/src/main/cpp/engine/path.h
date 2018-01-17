#pragma once

#include <vector>
#include <memory>
#include "../pch.h"

namespace Engine {

    template <typename T>
    class Traversable
    {
    protected:

        float m_length;

    public:

        Traversable(float length)
            : m_length(length)
        {
        }

        virtual ~Traversable() {}

        float getLength() { return m_length; }

        virtual T traverse(float dt) const = 0;
    };

    template <typename T>
    class Single : public Traversable<T>
    {
    private:

        T m_value;

    public:

        Single(float length, T value)
                : Traversable<T>(length)
                , m_value(value)
        {
        }

        virtual T traverse(float dt) const override { return m_value; }
    };

    template <typename T>
    class Linear : public Traversable<T>
    {
    private:

        T m_start;
        T m_end;

    public:

        Linear(float length, T start, T end)
                : Traversable<T>(length)
                , m_start(start)
                , m_end(end)
        {
        }

        virtual T traverse(float dt) const override
        {
            if(dt <= 0.0f)
                return m_start;
            if(dt >= 1.0f)
                return m_end;

            return (m_start * (1.0f - dt)) + (m_end * dt);
        }
    };

    template <typename T>
    class Bezier : public Traversable<T>
    {
    private:

        std::vector<T> m_points;

    public:

        Bezier(float length, UINT nrPoints, const T* pPoints)
                :Traversable<T>(length)
        {
            m_points = std::vector<T>(pPoints, pPoints+nrPoints);
        }

        virtual T traverse(float dt) const override
        {
            if(dt <= 0.0f)
                return m_points[0];
            if(dt >= 1.0f)
                return m_points[m_points.size()-1];

            std::vector<T> points = m_points;
            UINT i = static_cast<UINT>(points.size())-1;
            while(i > 0 )
            {
                for(UINT j=0; j<i; ++j)
                    points[j] += (points[j+1] - points[j]) * dt;

                --i;
            }

            return points[0];
        }
    };

    template <typename T>
    class Path
    {
    private:

        typedef std::shared_ptr<Traversable<T>> traversable_ptr;

        std::vector<traversable_ptr> m_traversables;

        float m_totalLength;

    public:

        Path()
            : m_totalLength(0)
        {
        }

        void add(float length, UINT nrControlPoints, const T* pControlPoints)
        {
            traversable_ptr p;
            switch (nrControlPoints)
            {
                case 0:
                    return;
                case 1:
                    p = traversable_ptr(new Single<T>(length, pControlPoints[0]));
                case 2:
                    p = traversable_ptr(new Linear<T>(length, pControlPoints[0], pControlPoints[1]));
                    break;
                default:
                    p = traversable_ptr(new Bezier<T>(length, nrControlPoints, pControlPoints));
                    break;
            }

            m_totalLength += length;
            m_traversables.push_back(p);
        }

        T traverse(float time)
        {
            for(auto& it : m_traversables)
            {
                if(it->getLength() > time)
                {
                    float dt = time / it->getLength();
                    return it->traverse(dt);
                }
                else
                    time -= it->getLength();
            }

            return (*m_traversables.rbegin())->traverse(1.0f);
        }
    };

}