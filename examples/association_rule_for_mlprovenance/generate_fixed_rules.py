Ls = [
['beer', 'diaper'],


['sock', 'pillow', 'paintbrush', 'barbell'],
['pillow', 'barbell', 'paintbrush'],


['espresso', 'dumbbell', 'sock', 'diaper'],


['dumbbell', 'mitten', 'teapot', 'pillow'],
['mitten', 'dumbbell', 'teapot'],


['llama', 'pillow', 'tractor'],
['llama', 'espresso', 'pillow', 'dumbbell'],


['diaper', 'llama', 'mitten'],


['cup', 'dumbbell', 'diaper'],
['diaper', 'cup'],


['paintbrush', 'barbell', 'espresso', 'mitten'],
['barbell', 'paintbrush', 'mitten'],


['tractor', 'barbell', 'cup', 'paintbrush'],


['teapot', 'mitten', 'dumbbell', 'barbell'],
['cup', 'diaper', 'mitten', 'dumbbell', 'teapot'],


['llama', 'paintbrush', 'teapot', 'sock'],
['llama', 'teapot', 'tractor', 'sock', 'paintbrush'],


['barbell', 'diaper', 'espresso', 'pillow'],


['pillow', 'espresso', 'llama', 'mitten'],
['llama', 'mitten', 'pillow'],

]

import random

for L in Ls:
    print ','.join([str(len(L))] + L + ['%.5f' % random.random()])
