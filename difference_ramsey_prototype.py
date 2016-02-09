#==========================================================================
# Single process prototype for difference ramsey algorithm for two colors
# Jiayang Li
# Spring 2016
#==========================================================================

from itertools import combinations

class ColorSet:
	'''
	The class to represent a 2-color difference coloring.
	Each color has a list of differences corresponding to that color.
	'''
	def __init__(self, red, blue):
		self.red = red
		self.blue = blue

	def __str__(self):
		return "red: {0}, blue: {1}. ".format(str(self.red), str(self.blue))

	@staticmethod
	def avoids(diff_set, clique_size):
		checkset_size = clique_size - 1
		for checkset in combinations(diff_set, checkset_size):
			if ColorSet.clique_exists(diff_set, checkset, checkset_size):
				return False
		return True

	@staticmethod
	def found(lst, target):
		start = 0
		end = len(lst) - 1
		while start <= end:
			mid = (start + end) // 2
			if lst[mid] == target:
				return True
			if lst[mid] > target:
				end = mid - 1
			else:
				start = mid + 1
		return False

	@staticmethod
	def clique_exists(diff_set, checkset, checkset_size):
		for i in range(checkset_size):
			for j in range(i+1, checkset_size):
				if not ColorSet.found(diff_set, abs(checkset[i] - checkset[j])):
					return False
		return True

	def search(self, k, l, new_diff):
		'''
		Try to add new_diff, but still avoid red K_k or blue K_l.
		'''
		p = 0
		q = 0

		#========== debugging print statements ==============
		#print("For graph {0}. Trying to add {1}.".format(str(self), new_diff))
		#====================================================

		# try coloring new_diff red
		self.red.append(new_diff)
		if ColorSet.avoids(self.red, k):
			p = self.search(k, l, new_diff+1)
		self.red.pop()

		# try coloring new_diff blue
		self.blue.append(new_diff)
		if ColorSet.avoids(self.blue, l):
			q = self.search(k, l, new_diff+1)
		self.blue.pop()

		return max(p, q, new_diff)



def powerset(seq, start):
	'''
	Generate all subsets of the given list. Used in creating the initial
	good set.
	'''
	if start == len(seq) - 1:
		yield seq[start:]
		yield []
	else:
		add_term = seq[start]
		for sub_seq in powerset(seq, start+1):
			yield [add_term] + sub_seq
			yield sub_seq

def get_blue_set(all_diff, red):
	'''
	Get the blue set, given the red set.
	'''
	blue = []
	j = 0
	for i in range(len(all_diff)):
		next_d = all_diff[i]
		if j == len(red) or next_d != red[j]:
			blue.append(next_d)
		else:
			j += 1
	return blue

def initial_good_set(n):
	'''
	Create all possible colorings on n vertices.
	'''
	all_diff = list(range(1, n))
	for red in powerset(all_diff, 0):
		blue = get_blue_set(all_diff, red)
		yield ColorSet(red, blue)


def difference_ramsey_number(k, l):
	'''
	Return the difference ramsey number, D(k, l).
	'''
	m = min(k, l) - 1
	for g in initial_good_set(m):
		print("Results for {0}".format(str(g)))
		print(g.search(k, l, m))
		print()



