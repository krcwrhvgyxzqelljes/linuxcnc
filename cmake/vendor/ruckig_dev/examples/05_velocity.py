from copy import copy

from ruckig import InputParameter, OutputParameter, Result, Ruckig, ControlInterface


if __name__ == '__main__':
    # Create instances: the Ruckig OTG as well as input and output parameters
    otg = Ruckig(3, 0.01)  # DoFs, control cycle
    inp = InputParameter(3)
    out = OutputParameter(3)

    inp.control_interface = ControlInterface.Velocity

    inp.current_position = [0.0, 0.0, 0.5]
    inp.current_velocity = [3.0, -2.2, -0.5]
    inp.current_acceleration = [0.0, 2.5, -0.5]

    inp.target_velocity = [0.0, -0.5, -1.5]
    inp.target_acceleration = [0.0, 0.0, 0.5]

    inp.max_acceleration = [3.0, 2.0, 1.0]
    inp.max_jerk = [6.0, 6.0, 4.0]


    print('\t'.join(['t'] + [str(i) for i in range(otg.degrees_of_freedom)]))

    # Generate the trajectory within the control loop
    first_output, out_list = None, []
    res = Result.Working
    while res == Result.Working:
        res = otg.update(inp, out)

        print('\t'.join([f'{out.time:0.3f}'] + [f'{p:0.3f}' for p in out.new_position]))
        out_list.append(copy(out))

        out.pass_to_input(inp)

        if not first_output:
            first_output = copy(out)

    print(f'Calculation duration: {first_output.calculation_duration:0.1f} [µs]')
    print(f'Trajectory duration: {first_output.trajectory.duration:0.4f} [s]')

    # Plot the trajectory
    # from pathlib import Path
    # import sys
    # project_path = Path(__file__).parent.parent.absolute()
    # sys.path.append(str(project_path / 'test'))  # For plotter.py

    # from plotter import Plotter

    # Plotter.plot_trajectory(project_path / 'examples' / '05_trajectory.pdf', otg, inp, out_list, plot_jerk=False)
