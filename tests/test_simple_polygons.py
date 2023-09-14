import point_in_polygon as pinp
import math
import pytest


_TAU = math.tau

def test_containment():
    # simple square
    square_x = [0.5, 2.4, 2.4, 0.5]
    square_y = [0.5, 0.5, 2.4, 2.4]
    with pytest.raises(ValueError):
        assert pinp.point_in_polygon(4, square_x, square_y, 0.7, 1.3)
    # close the polygon
    square_x.append(square_x[0])
    square_y.append(square_y[0])
    n_vert = len(square_x)
    assert n_vert == len(square_y)
    assert pinp.point_in_polygon(n_vert, square_x, square_y, 0.7, 1.3)
    assert not pinp.point_in_polygon(n_vert, square_x, square_y, 0.7, 13.)
    # shifted unit circle
    n_vert = 100
    x = [i/n_vert * _TAU for i in range(n_vert)]
    circle_x = list(math.sin(t) - 1.4 for t in x)
    circle_y = list(math.cos(t) + 0.3 for t in x)
    with pytest.raises(ValueError):
        assert pinp.point_in_polygon(n_vert, circle_x, circle_y, -1.4, 1.1)
    n_vert += 1
    circle_x.append(circle_x[0])
    circle_y.append(circle_y[0])
    assert not pinp.point_in_polygon(n_vert, circle_x, circle_y, 0., 0.)
    assert pinp.point_in_polygon(n_vert, circle_x, circle_y, -1., 0.)
    assert pinp.point_in_polygon(n_vert, circle_x, circle_y, -1.4, 1.1)
    assert pinp.point_in_polygon(n_vert, circle_x, circle_y, -1.3, -0.6)
    assert not pinp.point_in_polygon(n_vert, circle_x, circle_y, -2.25, 1.)
    return


def batch_containment():
    def linspace(start, end, N, func=None):
        step = (end - start) / N
        values = [start + t * step for t in range(N+1)]
        if func:
            values = [func(t) for t in values]
        return values
    n_vert = 42
    circle_0_x = linspace(0, _TAU, n_vert, math.sin)
    circle_0_y = linspace(0, _TAU, n_vert, math.cos)
    n_vert = 123
    circle_1_x = linspace(0, _TAU, n_vert, lambda x: math.sin(x) * 2 + 1)
    circle_1_y = linspace(0, _TAU, n_vert, lambda x: math.cos(x) * 2 - 2)
    n_vert = 13
    circle_2_x = linspace(0, _TAU, n_vert, lambda x: math.sin(x) * 0.5 + 0.9)
    circle_2_y = linspace(0, _TAU, n_vert, lambda x: math.cos(x) * 0.5 + 0.1)
    concatenated_x = circle_0_x + circle_1_x + circle_2_x
    concatenated_y = circle_0_y + circle_1_y + circle_2_y
    # one polygon not closed
    with pytest.raises(ValueError):
        pinp.point_in_polygon_triplet(42, 120, 11, circle_0_x, circle_0_y,
                                circle_1_x[:-3], circle_1_y[:-3], circle_2_x, circle_2_y, 0., 0.)
    assert pinp.point_in_polygon_triplet(
        42, 123, 11, circle_0_x, circle_0_y, circle_1_x, circle_1_y,
        circle_2_x, circle_2_y, 0., 0.) == [True, False, False]
    assert pinp.point_in_polygon_triplet(
        42, 123, 11, circle_0_x, circle_0_y, circle_1_x, circle_1_y,
        circle_2_x, circle_2_y, 0.7, -0.3) == [True, True, True]
    assert pinp.point_in_polygon_triplet(
        42, 123, 11, circle_0_x, circle_0_y, circle_1_x, circle_1_y,
        circle_2_x, circle_2_y, -1., -1.) == [False, False, False]
    return


def test_edge_cases():
    #   simple square
    n_vert = 5
    square_x = [0.5, 2.4, 2.4, 0.5, 0.5]
    square_y = [0.5, 0.5, 2.4, 2.4, 0.5]
    #   corners
    # bottom/left
    assert pinp.point_in_polygon(n_vert, square_x, square_y, 0.5, 0.5)
    # bottom/right
    assert not pinp.point_in_polygon(n_vert, square_x, square_y, 0.5, 2.4)
    # top/left
    assert not pinp.point_in_polygon(n_vert, square_x, square_y, 2.4, 0.5)
    # top/right
    assert not pinp.point_in_polygon(n_vert, square_x, square_y, 2.4, 2.4)
    #   edges
    # left
    assert pinp.point_in_polygon(n_vert, square_x, square_y, 0.5, 0.94)
    # top
    assert not pinp.point_in_polygon(n_vert, square_x, square_y, math.pi/2, 2.4)
    # right
    assert not pinp.point_in_polygon(n_vert, square_x, square_y, 2.4, 1.123)
    # bottom
    assert pinp.point_in_polygon(n_vert, square_x, square_y, 2.399, 0.5)
    return
