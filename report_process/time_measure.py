import os
import pandas as pd

def get_data():
    dataset = []

    with open('tmp_res.txt', 'r') as f:
        data = f.read()

    data = data.split('@')
    for d in data:
        d = d.split('\n')
        if len(d) >= 6:
            sec_str = d[5].split()[1].split('m')[1][:-1].split(',')
            kern_time = (float(sec_str[0]) + float(sec_str[1]) / 1000)
            method, f, batch_size, its = (d[1].split())

            file_size = os.path.getsize(f)
            dataset.append((method, batch_size, file_size, its, kern_time))
        

    df = pd.DataFrame(dataset, columns=['method', 'batch_size', 'size', 'iterations', 'kernel_time'])
    return df