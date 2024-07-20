package client

import (
	"math/rand"
	"time"
)

type Operation[T any] func() (T, error)

type BackoffRetry struct {
	InitialInterval     int32
	Multiplier          float32
	RandomizationFactor float32
	MaxRetries          int32
}

func NewBackoffRetryDefault() *BackoffRetry {
	return &BackoffRetry{
		InitialInterval:     500,
		Multiplier:          1.5,
		RandomizationFactor: 0.5,
		MaxRetries:          10,
	}
}

func NewBackoffRetry(initialInterval int32, multiplier float32, randomizationFactor float32, maxRetries int32) *BackoffRetry {
	return &BackoffRetry{
		InitialInterval:     initialInterval,
		Multiplier:          multiplier,
		RandomizationFactor: randomizationFactor,
		MaxRetries:          maxRetries,
	}
}

func (b *BackoffRetry) SetInitialInterval(interval int32) {
	b.InitialInterval = interval
}

func (b *BackoffRetry) SetMultiplier(multiplier float32) {
	b.Multiplier = multiplier
}

func (b *BackoffRetry) SetRandomizationFactor(factor float32) {
	b.RandomizationFactor = factor
}

func (b *BackoffRetry) getRandomizedInterval(interval float32) float32 {
	delta := b.RandomizationFactor * interval
	minInterval := interval - delta
	maxInterval := interval + delta

	randomFactor := rand.Float32()
	return minInterval + (maxInterval-minInterval)*randomFactor
}

func Retry[T any](o Operation[T], b BackoffRetry) (T, error) {
	interval := b.InitialInterval
	var res T
	var err error
	var i int32 = 0

	for i <= b.MaxRetries {
		res, err = o()
		if err == nil {
			return res, nil
		}

		randomizedInterval := b.getRandomizedInterval(float32(interval))
		time.Sleep(time.Duration(randomizedInterval) * time.Millisecond)

		interval = int32(b.Multiplier * float32(interval))
		i++
	}

	return res, err
}
