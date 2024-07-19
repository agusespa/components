package client

type Backoff interface {
	SetInitialInterval(interval int)
	SetMultiplier(multiplier float32)
	SetRandomizationFactor(factor float32)
	Retry(req func())
}

type ExponentialBackoff struct {
	InitialInterval     int32
	Multiplier          float32
	RandomizationFactor float32
}

func NewExponentialBackoffDefault() *ExponentialBackoff {
	return &ExponentialBackoff{
		InitialInterval:     500,
		Multiplier:          1.5,
		RandomizationFactor: 0.5,
	}
}

func NewExponentialBackoff(initialInterval int32, multiplier float32, randomizationFactor float32) *ExponentialBackoff {
	return &ExponentialBackoff{
		InitialInterval:     initialInterval,
		Multiplier:          multiplier,
		RandomizationFactor: randomizationFactor,
	}
}

func (b *ExponentialBackoff) SetInitialInterval(interval int32) {
	b.InitialInterval = interval
}

func (b *ExponentialBackoff) SetMultiplier(multiplier float32) {
	b.Multiplier = multiplier
}

func (b *ExponentialBackoff) SetRandomizationFactor(factor float32) {
	b.RandomizationFactor = factor
}
