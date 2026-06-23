class KalmanFilter1D:
    def __init__(self, x, P, Q, R):
        self.x = x
        self.P = P
        self.Q = Q
        self.R = R

    def predict(self, u=0.0) -> float:
        self.x = self.x + u
        self.P = self.P + self.Q
        return self.x

    def update(self, z) -> float:
        K = self.P / (self.P + self.R)
        self.x = self.x + K * (z - self.x)
        self.P = (1 - K) * self.P
        return self.x
