import numpy as np


class Tensor:
    def __init__(self, value, back_op=None):
        self.value = value
        self.grad = np.zeros_like(value)
        self.back_op = back_op

    def __str__(self):
        str_val = str(self.value)
        str_val = '\t' + '\n\t'.join(str_val.split('\n'))
        str_bwd = str(self.back_op.__class__.__name__)
        return 'Tensor(\n' + str_val + '\n\tbwd: ' + str_bwd + '\n)'

    @property
    def shape(self):
        return self.value.shape

    def backward(self, deltas=None):
        if deltas is not None:
            assert deltas.shape == self.value.shape, f'Expected gradient with shape {self.value.shape}, got {deltas.shape}'

            self.grad = deltas
            if self.back_op is not None:
               self.back_op.backward(self.grad)
        else:
            if self.shape != tuple() and np.prod(self.shape) != 1:
                raise ValueError(f'Can only backpropagate a scalar, got shape {self.shape}')

            if self.back_op is None:
                raise ValueError(f'Cannot start backpropagation from a leaf!')

            self.grad = np.ones_like(self.value)
            self.back_op.backward(self.grad)


class SumPropagation:
    def __init__(self, tensor):
        self.tensor = tensor

    def backward(self, grad):
        self.tensor.grad += grad * np.ones_like(self.tensor.value)
        self.tensor.backward(self.tensor.grad)


class ReluPropagation:
    def __init__(self, tensor):
        self.tensor = tensor

    def backward(self, grad):
        map = self.tensor.value >= 0.0
        self.tensor.grad += np.multiply(grad, map)

        self.tensor.backward(self.tensor.grad)


class AddSubPropagation:
    def __init__(self, left, right, op):
        self.left = left
        self.right = right
        self.op = op

    def backward(self, grad):
        self.left.grad += grad
        self.right.grad += self.op * grad

        self.left.backward(self.left.grad)
        self.right.backward(self.right.grad)


class MultiplyPropagation:
    def __init__(self, left, right):
        self.left = left
        self.right = right

    def backward(self, grad):
        self.left.grad += np.multiply(self.right.value, grad)
        self.right.grad += np.multiply(self.left.value, grad)

        self.left.backward(self.left.grad)
        self.right.backward(self.right.grad)


class DotPropagation:
    def __init__(self, left, right):
        self.left = left
        self.right = right

    def backward(self, grad):
        self.left.grad += np.dot(grad, self.right.value.T)
        self.right.grad += np.dot(self.left.value.T, grad)

        self.left.backward(self.left.grad)
        self.right.backward(self.right.grad)


def sui_sum(tensor):
    return Tensor(np.sum(tensor.value), SumPropagation(tensor))


def add(a, b):
    return Tensor(np.add(a.value, b.value), AddSubPropagation(a, b, 1))


def subtract(a, b):
    return Tensor(np.subtract(a.value, b.value), AddSubPropagation(a, b, -1))


def multiply(a, b):
    return Tensor(np.multiply(a.value, b.value), MultiplyPropagation(a, b))


def relu(tensor):
    return Tensor(np.maximum(tensor.value, 0.0), ReluPropagation(tensor))


def dot_product(a, b):
    return Tensor(np.dot(a.value, b.value), DotPropagation(a, b))
