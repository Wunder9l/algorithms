# article with description https://habr.com/ru/post/209610/
from math import exp
from random import random


class Annealing:
    def __init__(self, energy_func, next_state_func, temperature_func):
        """
        Annealing simulation class as a stochastic method of optimization task. Finds decision with minimal energy level
        :param energy_func: computes energy for specific state
        :param next_state_func: generates next state based on previous one
        :param temperature_func: generates next temperature based on current temperature and iteration number
        """
        self.energy_func = energy_func
        self.next_state_func = next_state_func
        self.temperature_func = temperature_func

    def simulate(self, t_start, t_end, state, max_iteration=1000000, on_each_iteration_callback=None):
        """
        Run simulation
        :param t_start: start temperature for annealing
        :param t_end: end temperature for annealing
        :param state: initial state
        :param max_iteration: max number of iterations (if t_end reaches first there will be less iterations)
        :param on_each_iteration_callback: callback with signature (state, energy_level, temperature) called on each
        iteration for debug
        :return: pair of (optimal state, energy of optimal state)
        """
        t = t_start
        for i in range(1, max_iteration + 1):
            prev_energy = self.energy_func(state)
            next_state = self.next_state_func(state)
            next_energy = self.energy_func(next_state)
            state = next_state if self.do_state_change(prev_energy, next_energy, t) else state
            t = self.temperature_func(t, i)
            if on_each_iteration_callback:
                on_each_iteration_callback(state, self.energy_func(state), t)
            if t <= t_end:
                break
        return state, self.energy_func(state)

    @staticmethod
    def do_state_change(prev_energy, next_energy, t):
        """
        Replies on question should state be changed to new one
        :param prev_energy: energy level of previous state
        :param next_energy: energy level of next state
        :param t: current temperature
        :return: bool
        """
        if prev_energy > next_energy:
            return True
        else:
            probability = exp((prev_energy - next_energy) / t)
            return probability >= random()
