package utils

import (
	"errors"
	"math/rand"
	"sync"
	"testing"
	"time"
)

func TestAsyncQueue(t *testing.T) {

	t.Run("Respects concurrency limit", func(t *testing.T) {
		queue := NewAsyncQueue(2)
		var mu sync.Mutex
		activeCount := 0
		maxActiveCount := 0
		wg := sync.WaitGroup{}

		for i := 0; i < 5; i++ {
			wg.Add(1)
			queue.Enqueue(func() (interface{}, error) {
				defer wg.Done()

				mu.Lock()
				activeCount++
				if activeCount > maxActiveCount {
					maxActiveCount = activeCount
				}
				mu.Unlock()

				time.Sleep(50 * time.Millisecond)

				mu.Lock()
				activeCount--
				mu.Unlock()

				return nil, nil
			})
		}

		wg.Wait()

		if maxActiveCount > 2 {
			t.Errorf("Concurrency limit not respected. Max active count: %d", maxActiveCount)
		}
	})

	t.Run("Tasks execute in order", func(t *testing.T) {
		queue := NewAsyncQueue(2)
		results := make([]int, 0)
		var mu sync.Mutex
		wg := sync.WaitGroup{}

		for i := 0; i < 5; i++ {
			wg.Add(1)
			j := i
			queue.Enqueue(func() (interface{}, error) {
				defer wg.Done()
				mu.Lock()
				defer mu.Unlock()
				results = append(results, j)
				return nil, nil
			})
		}

		wg.Wait()

		if len(results) != 5 {
			t.Errorf("Expected 5 results, got %d", len(results))
		}
		for i, v := range results {
			if v != i {
				t.Errorf("Tasks not executed in order. Expected %d, got %d at position %d", i, v, i)
			}
		}
	})

	t.Run("Returns correct results", func(t *testing.T) {
		queue := NewAsyncQueue(3)
		resultChans := make([]<-chan interface{}, 5)
		results := make([]interface{}, 5)
		wg := sync.WaitGroup{}

		for i := 0; i < 5; i++ {
			wg.Add(1)
			j := i
			resultChans[i], _ = queue.Enqueue(func() (interface{}, error) {
				defer wg.Done()
				time.Sleep(time.Duration(rand.Intn(500)) * time.Millisecond)
				return j * 2, nil
			})
		}

		for i, ch := range resultChans {
			go func(i int, ch <-chan interface{}) {
				results[i] = <-ch
			}(i, ch)
		}

		wg.Wait()

		for i, result := range results {
			if result != i*2 {
				t.Errorf("Incorrect result. Expected %d, got %v", i*2, result)
			}
		}
	})

	t.Run("Handles errors", func(t *testing.T) {
		queue := NewAsyncQueue(1)
		task := func(result string, delay time.Duration) func() (interface{}, error) {
			return func() (interface{}, error) {
				time.Sleep(delay)
				return result, nil
			}
		}

		taskThatFails := func(delay time.Duration) func() (interface{}, error) {
			return func() (interface{}, error) {
				time.Sleep(delay)
				return nil, errors.New("Task failed")
			}
		}

		result1, _ := queue.Enqueue(task("Result 1", 100*time.Millisecond))
		_, err2 := queue.Enqueue(taskThatFails(200 * time.Millisecond))
		result3, _ := queue.Enqueue(task("Result 2", 50*time.Millisecond))
		result4, _ := queue.Enqueue(task("Result 3", 150*time.Millisecond))

		results := []interface{}{
			<-result1,
			<-err2,
			<-result3,
			<-result4,
		}

		expected := []interface{}{"Result 1", errors.New("Task failed"), "Result 2", "Result 3"}

		for i, r := range results {
			switch exp := expected[i].(type) {
			case string:
				if res, ok := r.(string); !ok || res != exp {
					t.Errorf("Expected %v, got %v", exp, r)
				}
			case error:
				if err, ok := r.(error); !ok || err.Error() != exp.Error() {
					t.Errorf("Expected error '%v', got '%v'", exp.Error(), err.Error())
				}
			}
		}
	})
}
