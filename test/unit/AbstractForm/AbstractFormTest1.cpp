#include "AbstractFormTest1.hpp"

#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Gui/AbstractForm.hpp>

using namespace BlendInt;

AbstractFormTest1::AbstractFormTest1()
: testing::Test()
{
	// TODO: add constructor code
}

AbstractFormTest1::~AbstractFormTest1()
{
	// TODO: add destructor code
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result: 
 */
TEST_F(AbstractFormTest1, GenerateRectVertices01)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;

    AbstractForm::GenerateRectVertices (Size(100, 100), 1.f, &inner, &outer);

    ASSERT_TRUE(inner.size() == 12 && outer.size() == 8);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices01)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundNone, 0.f, &inner, &outer);

    ASSERT_TRUE(sum.total == 4 && inner.size() == 12 && outer.size() == 8);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices02)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundTopLeft, 5.f, &inner, &outer);

    ASSERT_TRUE(sum.total == 12 && inner.size() == 28 && outer.size() == 24);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices03)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundTopLeft | RoundTopRight, 5.f, &inner, &outer);

    ASSERT_TRUE(sum.total == 20 && inner.size() == 44 && outer.size() == 40);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices04)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundTopLeft | RoundTopRight | RoundBottomLeft, 5.f, &inner, &outer);

    ASSERT_TRUE(sum.total == 28 && inner.size() == 60 && outer.size() == 56);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices05)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundAll, 5.f, &inner, &outer);

    ASSERT_TRUE(sum.total == 36 && inner.size() == 76 && outer.size() == 72);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices06)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundNone, 0.f, color, 5, -5, Vertical, &inner, &outer);

    ASSERT_TRUE(sum.total == 4 && inner.size() == 36 && outer.size() == 8);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices07)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundBottomLeft, 5.f, color, 5, -5, Vertical, &inner, &outer);

    ASSERT_TRUE(sum.total == 12 && inner.size() == 84 && outer.size() == 24);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices08)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundBottomLeft | RoundBottomRight, 5.f, color, 5, -5, Vertical, &inner, &outer);

    ASSERT_TRUE(sum.total == 20 && inner.size() == 132 && outer.size() == 40);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices09)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundBottomLeft | RoundBottomRight | RoundTopLeft, 5.f, color, 5, -5, Vertical, &inner, &outer);

    ASSERT_TRUE(sum.total == 28 && inner.size() == 180 && outer.size() == 56);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateRoundVertices10)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundAll, 5.f, color, 5, -5, Vertical, &inner, &outer);

    ASSERT_TRUE(sum.total == 36 && inner.size() == 228 && outer.size() == 72);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateTriangleStripVertices01)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundNone, 0.f, &inner, &outer);

    std::vector<GLfloat> strip;

    AbstractForm::GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

    ASSERT_TRUE(sum.total == 4 && inner.size() == 12 && outer.size() == 8 && strip.size() == 20);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateTriangleStripVertices02)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundBottomLeft, 5.f, color, 5, -5, Vertical, &inner, &outer);

    std::vector<GLfloat> strip;

    AbstractForm::GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

    ASSERT_TRUE(sum.total == 12 && inner.size() == 84 && outer.size() == 24 && strip.size() == 52);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateTriangleStripVertices03)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundBottomLeft | RoundBottomRight, 5.f, color, 5, -5, Vertical, &inner, &outer);

    std::vector<GLfloat> strip;

    AbstractForm::GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

    ASSERT_TRUE(sum.total == 20 && inner.size() == 132 && outer.size() == 40 && strip.size() == 84);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateTriangleStripVertices04)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundBottomLeft | RoundBottomRight | RoundTopLeft, 5.f, color, 5, -5, Vertical, &inner, &outer);

    std::vector<GLfloat> strip;

    AbstractForm::GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

    ASSERT_TRUE(sum.total == 28 && inner.size() == 180 && outer.size() == 56 && strip.size() == 116);
}

/**
 * test AbstractForm::GenerateRectVertices() method
 *
 * Expected result:
 */
TEST_F(AbstractFormTest1, GenerateTriangleStripVertices05)
{
    std::vector<GLfloat> inner;
    std::vector<GLfloat> outer;
    VerticesSum sum;
    Color color;

    sum = AbstractForm::GenerateRoundVertices (Size(100, 100), 1.f, RoundAll, 5.f, color, 5, -5, Vertical, &inner, &outer);

    std::vector<GLfloat> strip;

    AbstractForm::GenerateTriangleStripVertices(inner, outer, sum.total, &strip);

    ASSERT_TRUE(sum.total == 36 && inner.size() == 228 && outer.size() == 72 && strip.size() == 148);
}
