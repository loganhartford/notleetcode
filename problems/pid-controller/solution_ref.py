class PID:
    def __init__(self, kp, ki, kd, dt, out_min=None, out_max=None):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.dt = dt
        self.out_min = out_min
        self.out_max = out_max
        self.integral = 0.0
        self.prev_error = 0.0

    def update(self, setpoint, measurement) -> float:
        error = setpoint - measurement
        self.integral += error * self.dt
        derivative = (error - self.prev_error) / self.dt
        output = self.kp * error + self.ki * self.integral + self.kd * derivative

        if self.out_max is not None and output > self.out_max:
            self.integral -= error * self.dt  # anti-windup
            output = self.out_max
        elif self.out_min is not None and output < self.out_min:
            self.integral -= error * self.dt
            output = self.out_min

        self.prev_error = error
        return output

    def reset(self) -> None:
        self.integral = 0.0
        self.prev_error = 0.0
