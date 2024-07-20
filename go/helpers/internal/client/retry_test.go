package client

import (
	"errors"
	"testing"
	"time"
)

func TestNewExponentialBackoffDefault(t *testing.T) {
	eb := NewBackoffRetryDefault()

	if eb.InitialInterval != 500 {
		t.Errorf("Expected InitialInterval to be 500, got %d", eb.InitialInterval)
	}

	if eb.Multiplier != 1.5 {
		t.Errorf("Expected Multiplier to be 1.5, got %f", eb.Multiplier)
	}

	if eb.RandomizationFactor != 0.5 {
		t.Errorf("Expected RandomizationFactor to be 0.5, got %f", eb.RandomizationFactor)
	}
}

func TestNewExponentialBackoff(t *testing.T) {
	tests := []struct {
		name                string
		initialInterval     int32
		multiplier          float32
		randomizationFactor float32
		maxRetries          int32
	}{
		{"Custom Values", 1000, 2.0, 0.3, 6000},
		{"Zero Values", 0, 0, 0, 0},
		{"Mixed Values", 750, 1.75, 0, 6000},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			eb := NewBackoffRetry(tt.initialInterval, tt.multiplier, tt.randomizationFactor, tt.maxRetries)

			if eb.InitialInterval != tt.initialInterval {
				t.Errorf("Expected InitialInterval to be %d, got %d", tt.initialInterval, eb.InitialInterval)
			}

			if eb.Multiplier != tt.multiplier {
				t.Errorf("Expected Multiplier to be %f, got %f", tt.multiplier, eb.Multiplier)
			}

			if eb.RandomizationFactor != tt.randomizationFactor {
				t.Errorf("Expected RandomizationFactor to be %f, got %f", tt.randomizationFactor, eb.RandomizationFactor)
			}
		})
	}
}

func TestGetRandomizedInterval(t *testing.T) {
	backoff := BackoffRetry{
		RandomizationFactor: 0.5,
	}

	interval := float32(1000)
	for i := 0; i < 100; i++ {
		randomizedInterval := backoff.getRandomizedInterval(interval)

		delta := backoff.RandomizationFactor * interval
		minInterval := interval - delta
		maxInterval := interval + delta

		if randomizedInterval < minInterval || randomizedInterval > maxInterval {
			t.Errorf("Randomized interval %f out of expected range [%f, %f]", randomizedInterval, minInterval, maxInterval)
		}
	}
}

func TestRetry(t *testing.T) {
	backoff := BackoffRetry{
		MaxRetries:          8,
		InitialInterval:     500,
		RandomizationFactor: 0.5,
		Multiplier:          1.5,
	}

	expectedTimeRange := func(initial int32, multiplier float32, retries int32) (min, max time.Duration) {
		min, max = 0*time.Millisecond, 0*time.Millisecond
		interval := float32(initial)
		var i int32
		for i = 0; i <= retries; i++ {
			delta := interval * backoff.RandomizationFactor
			min += time.Duration(interval-delta) * time.Millisecond
			max += time.Duration(interval+delta) * time.Millisecond
			interval = interval * multiplier
		}
		return
	}

	// Test case 1: Operation succeeds on first try
	t.Run("SuccessOnFirstTry", func(t *testing.T) {
		count := 0
		op := func() (interface{}, error) {
			count++
			return 42, nil
		}

		start := time.Now()
		result, err := Retry(op, backoff)
		end := time.Now()
		elapsed := end.Sub(start)

		if err != nil {
			t.Errorf("Expected no error, got %v", err)
		}
		if result != 42 {
			t.Errorf("Expected result 42, got %d", result)
		}
		if count != 1 {
			t.Errorf("Expected operation to be called once, was called %d times", count)
		}

		expectedMaxDuration := time.Duration(backoff.InitialInterval) * time.Millisecond
		if elapsed >= expectedMaxDuration {
			t.Errorf("Elapsed time %v surpassed expected max of %d ns", elapsed, expectedMaxDuration)
		}
	})

	// Test case 2: Operation succeeds after a few retries
	t.Run("SuccessAfterFewRetries", func(t *testing.T) {
		count := 0
		op := func() (interface{}, error) {
			count++
			if count < 3 {
				return nil, errors.New("temporary error")
			}
			return "success", nil
		}

		start := time.Now()
		result, err := Retry(op, backoff)
		end := time.Now()
		elapsed := end.Sub(start)

		if err != nil {
			t.Errorf("Expected no error, got %v", err)
		}
		if result != "success" {
			t.Errorf("Expected result 'success', got '%s'", result)
		}
		if count != 3 {
			t.Errorf("Expected operation to be called 3 times, was called %d times", count)
		}

		min, max := expectedTimeRange(backoff.InitialInterval, backoff.Multiplier, int32(count-1))
		if elapsed < min || elapsed > max {
			t.Errorf("Elapsed time %v out of expected range [%v, %v]", elapsed, min, max)
		}
	})

	// Test case 2: Operation succeeds after some retries
	t.Run("SuccessAfterSomeRetries", func(t *testing.T) {
		count := 0
		op := func() (interface{}, error) {
			count++
			if count < 8 {
				return nil, errors.New("temporary error")
			}
			return "success", nil
		}

		start := time.Now()
		result, err := Retry(op, backoff)
		end := time.Now()
		elapsed := end.Sub(start)

		if err != nil {
			t.Errorf("Expected no error, got %v", err)
		}
		if result != "success" {
			t.Errorf("Expected result 'success', got '%s'", result)
		}
		if count != 8 {
			t.Errorf("Expected operation to be called 8 times, was called %d times", count)
		}

		min, max := expectedTimeRange(backoff.InitialInterval, backoff.Multiplier, int32(count-1))
		if elapsed < min || elapsed > max {
			t.Errorf("Elapsed time %v out of expected range [%v, %v]", elapsed, min, max)
		}
	})

	// Test case 3: Operation never succeeds
	t.Run("NeverSucceeds", func(t *testing.T) {
		count := 0
		op := func() (interface{}, error) {
			count++
			return nil, errors.New("persistent error")
		}

		start := time.Now()
		_, err := Retry(op, backoff)
		end := time.Now()
		elapsed := end.Sub(start)

		if err == nil {
			t.Error("Expected an error, got nil")
		}
		if count != int(backoff.MaxRetries+1) {
			t.Errorf("Expected operation to be called %d times, was called %d times", backoff.MaxRetries+1, count)
		}

		min, max := expectedTimeRange(backoff.InitialInterval, backoff.Multiplier, backoff.MaxRetries)
		if elapsed < min || elapsed > max {
			t.Errorf("Elapsed time %v out of expected range [%v, %v]", elapsed, min, max)
		}
	})
}
