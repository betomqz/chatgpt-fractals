from manim import *


class NewtonsMethod(Scene):
    def construct(self):
        # set up graph
        ax = Axes(
            x_range=[-0.4, 16],
            y_range=[-6, 8],
            axis_config={"include_tip": True}
        )
        labels = ax.get_axis_labels(x_label="x", y_label="f(x)")

        def func(x):
            return 0.03*(x-1)*(x-10)*(x-13)

        def func_prime(x):
            return 0.09*x**2 - 1.44*x + 4.59

        graph = ax.plot(func, x_range=[-0.4, 16], color=BLUE)

        # Add initial point (x0, 0)
        x0 = ValueTracker(5.2)
        x0_dot = Dot(
            ax.coords_to_point(x0.get_value(), 0),
            color=YELLOW
        )
        x0_dot.add_updater(
            lambda m: m.move_to(ax.coords_to_point(x0.get_value(), 0))
        )
        x0_label = DecimalNumber(
            x0.get_value(),
            color=YELLOW,
            num_decimal_places=2,
            show_ellipsis=False
        )
        x0_label.next_to(x0_dot, DOWN)
        x0_label.add_updater(
            lambda m: m.set_value(x0.get_value())
        )
        x0_label.add_updater(
            lambda m: m.next_to(x0_dot, DOWN)
        )

        # Add f(x0)
        fx0_dot = Dot(
            ax.coords_to_point(x0.get_value(), func(x0.get_value()))
        )
        fx0_dot.add_updater(
            lambda m: m.move_to(ax.coords_to_point(
                x0.get_value(), func(x0.get_value())))
        )
        fx0_label = MathTex(
            "f(x_i)"
        )
        fx0_label.next_to(fx0_dot, UP)
        fx0_label.add_updater(
            lambda m: m.next_to(fx0_dot, UP)
        )

        # get tangent line
        def get_tang_line():
            x_izq = 0
            y_izq = func(x0.get_value()) - \
                func_prime(x0.get_value())*x0.get_value()

            if (abs(y_izq) > 6):
                x_izq = x0.get_value() - 2
                y_izq = func_prime(x0.get_value())*(-2) + func(x0.get_value())

            tang_line = Line(
                start=ax.coords_to_point(x_izq, y_izq),
                end=ax.coords_to_point(
                    16, func(x0.get_value()) + func_prime(x0.get_value())*(16-x0.get_value())),
                color=MAROON
            )
            return tang_line

        def get_vert_line():
            vert_line = DashedLine(
                start=ax.coords_to_point(x0.get_value(), 0),
                end=ax.coords_to_point(x0.get_value(), func(x0.get_value()))
            )
            return vert_line

        vert_line = always_redraw(get_vert_line)

        self.play(Create(ax), Create(labels), Create(graph))
        self.wait(1)
        self.play(FadeIn(x0_dot), FadeIn(x0_label))
        self.wait(1)
        self.play(Create(vert_line))
        self.play(FadeIn(fx0_dot))
        tang_line = get_tang_line()
        self.play(Create(tang_line))
        self.wait(2)
        # self.play(Uncreate(vert_line))

        for i in range(5):
            x1 = x0.get_value() - func(x0.get_value()) / func_prime(x0.get_value())
            self.play(x0.animate.set_value(x1))
            self.play(Uncreate(tang_line))
            if i < 4:
                tang_line = get_tang_line()
                self.play(Create(tang_line))
                self.wait(2)

        self.play(x0.animate.set_color(GREEN),
                  x0_label.animate.set_color(GREEN))
        self.wait(2)
