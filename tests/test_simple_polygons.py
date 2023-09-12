import point_in_polygon as pinp
import math


def test_containment():
    # simple square
    square_x = [0.5, 2.4, 2.4, 0.5]
    square_y = [0.5, 0.5, 2.4, 2.4]
    assert pinp.point_in_polygon(4, square_x, square_y, 0.7, 1.3)
    assert not pinp.point_in_polygon(4, square_x, square_y, 0.7, 13.)
    # shifted unit circle
    x = [i/100 * math.tau for i in range(100)]
    circle_x = list(math.sin(t) - 1.4 for t in x)
    circle_y = list(math.cos(t) + 0.3 for t in x)
    assert not pinp.point_in_polygon(100, circle_x, circle_y, 0., 0.)
    assert pinp.point_in_polygon(100, circle_x, circle_y, -1., 0.)
    assert pinp.point_in_polygon(100, circle_x, circle_y, -1.4, 1.1)
    assert pinp.point_in_polygon(100, circle_x, circle_y, -1.3, -0.6)
    assert not pinp.point_in_polygon(100, circle_x, circle_y, -2.25, 1.)
    return


def test_edge_cases():
    #   simple square
    square_x = [0.5, 2.4, 2.4, 0.5, 0.5]
    square_y = [0.5, 0.5, 2.4, 2.4, 0.5]
    #   corners
    # bottom/left
    assert pinp.point_in_polygon(5, square_x, square_y, 0.5, 0.5)
    # bottom/right
    assert not pinp.point_in_polygon(5, square_x, square_y, 0.5, 2.4)
    # top/left
    assert not pinp.point_in_polygon(5, square_x, square_y, 2.4, 0.5)
    # top/right
    assert not pinp.point_in_polygon(5, square_x, square_y, 2.4, 2.4)
    #   edges
    # left
    assert pinp.point_in_polygon(5, square_x, square_y, 0.5, 0.94)
    # top
    assert not pinp.point_in_polygon(5, square_x, square_y, math.pi/2, 2.4)
    # right
    assert not pinp.point_in_polygon(5, square_x, square_y, 2.4, 1.123)
    # bottom
    assert pinp.point_in_polygon(5, square_x, square_y, 2.399, 0.5)
    return
