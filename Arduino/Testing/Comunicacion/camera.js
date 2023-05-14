const OpenMV = require('./openmv.js');

class Camera {
    constructor (width = 320, height = 240) {
        this.width = width;
        this.height = height;
    }
    openMV = new OpenMV(this.width, this.height);
    #queryString = ''; 
    #ox = -1;
    #oy = -1;
    #bx = -1;
    #by = -1;
    #yx = -1;
    #yy = -1;

    call () {
        const result = this.openMV.data(this.#queryString);
        this.save(result);
    }
    add (string = '') {
        let query = [];
        for (let i = 0; i < this.#queryString.length; i += 2) {
            query.push(this.#queryString.slice(i, i + 2));
        }
        if (query.includes(string)) return;
        this.#queryString += string;
    }
    save (result = '') {
        let dividedResults = result.split('-');
        dividedResults.shift();
        dividedResults.forEach((value) => {
            value = value.split(':');
            switch (value[0]) {
                case 'ox': this.#ox = value[1]; break;
                case 'oy': this.#oy = value[1]; break;
                case 'bx': this.#bx = value[1]; break;
                case 'by': this.#by = value[1]; break;
                case 'yx': this.#yx = value[1]; break;
                case 'yy': this.#yy = value[1]; break;
            }
        })
    }

    ox () { return this.#ox }
    oy () { return this.#oy }
    bx () { return this.#bx }
    by () { return this.#by }
    yx () { return this.#yx }
    yy () { return this.#yy }
}

module.exports = Camera;