from random import random, randrange, shuffle
from math import sqrt

from annealing import Annealing


def euclidean(p1, p2):
    return sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)


class DebugStorage:
    def __init__(self):
        self.temperatures = []
        self.energies = []


class TravelingSalesmanProblem:
    def __init__(self, max_x, max_y, cities_number):
        self.cities = [(random() * max_x, random() * max_y) for _ in range(cities_number)]
        self.annealing = Annealing(self.distance, self.generate_route, self.next_temperature)

    def solve(self, t_start, t_finish, max_iter=1000000, debug=True):
        self.start_temperature = t_start
        state = list(range(len(self.cities)))
        if debug:
            self.debug_storage = DebugStorage()
        return self.annealing.simulate(t_start, t_finish, state, max_iter, self.debug_callback if debug else None)

    def distance(self, state):
        return sum(euclidean(self.cities[state[i - 1]], self.cities[state[i]]) for i in range(len(state)))

    @staticmethod
    def generate_route(state):
        """
        Reverse order of subsequence inside state
        :param state: cities to visit
        :return: state
        """
        a = randrange(len(state))
        b = randrange(len(state))
        if a == b:
            return state
        a, b = min(a, b), max(a, b)
        return state[:a] + list(reversed(state[a:b])) + state[b:]

    def next_temperature(self, t, i):
        return self.start_temperature / i

    def debug_callback(self, state, energy, t):
        self.debug_storage.energies.append(energy)
        self.debug_storage.temperatures.append(t)

    def plot(self, state):
        import matplotlib.pyplot as plt
        fig, ax = plt.subplots(2, 2)
        ax[1, 0].plot(self.debug_storage.temperatures)
        ax[1, 0].set_title("Temperature's change")
        ax[1, 1].plot(self.debug_storage.energies)
        ax[1, 1].set_title("Energy's change")

        self._plot_scatter_with_lines(ax[0, 0], [c[0] for c in self.cities], [c[1] for c in self.cities], 'First iteration')

        x_ordered = [self.cities[i][0] for i in state]
        y_ordered = [self.cities[i][1] for i in state]
        self._plot_scatter_with_lines(ax[0, 1], x_ordered, y_ordered, 'Last iteration')

        plt.show()

    @staticmethod
    def _plot_scatter_with_lines(ax, x, y, title):
        ax.plot(x, y, 'C3', zorder=1, lw=3)
        ax.scatter(x, y, s=60, zorder=2)
        ax.set_title(title)


if __name__ == '__main__':
    problem = TravelingSalesmanProblem(10, 10, 50)
    state, final_res = problem.solve(10, 0.0, 100000)
    problem.plot(state)
